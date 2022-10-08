#include "idt.h"
#include "config.h"
#include "kmemory.h"
#include "assert.h"
#include "printk.h"
#include "io.h"
#include "task.h"
#include "status.h"
#include "process.h"

struct idt_desc64 idt_descriptors64[TOTAL_INTERRUPTS];
struct idtr_desc64 idtr_descriptor64;
static ISR80H_COMMAND isr80h_commands[OS_MAX_ISR80H_COMMANDS];
static INTERRUPT_CALLBACK_FUNCTION interrupt_callbacks[TOTAL_INTERRUPTS];
static uint64_t current_ticks;

extern void no_interrupt_handler();
extern void isr80h_wrapper();
extern void load_idt(struct idtr_desc64* ptr);
extern void* interrupt_pointers[TOTAL_INTERRUPTS];
extern void set_kernel_registers();

const char* exception_messages[] = {
    "Divide by zero Error", // 0
    "Debug", // 1
    "Non maskable Interrupt", // 2
    "Breakpoint", // 3
    "Overflow", // 4
    "Bound Range Exceeded", // 5
    "Invalid Opcode", // 6
    "Device Not Available", // 7
    "Double Fault", // 8
    "Coprocessor Segment Overrun", // 9
    "Bad TSS", // 10
    "Segment Not Present", // 11
    "Stack Fault", //12
    "General Protection Fault", // 13
    "Page Fault", // 14
    "Reserved", // 15
    "x87 Floating-Point Exception", // 16
    "Alignment Check", // 17
    "Machine Check", // 18
    "SIMD Floating-Point Exception", // 19
    "Virtualization Exception", // 20
    "Control Protection Exception", // 21
    "Reserved", // 22
    "Reserved", // 23
    "Reserved", // 24
    "Reserved", // 25
    "Reserved", // 26
    "Reserved", // 27
    "Hypervisor Injection Exception", // 28
    "VMM Communication Exception", // 29
    "Security Exception", // 30
    "Reserved" // 31
};


uint64_t get_current_ticks()
{
    return current_ticks;
}

void idt_set(int interrupt_no, void* address, uint8_t attribute)
{
    assert(interrupt_no >= 0);
    assert(interrupt_no < TOTAL_INTERRUPTS);
    struct idt_desc64* idt_desc64 = &idt_descriptors64[interrupt_no];
    /**
        Offset: A 64-bit value, split in three parts. It represents the address of the entry point of the Interrupt Service Routine.
        Selector: A Segment Selector with multiple fields which must point to a valid code segment in your GDT.
        IST: A 3-bit value which is an offset into the Interrupt Stack Table, which is stored in the Task State Segment. If the bits are all set to zero, the Interrupt Stack Table is not used.
        Gate Type: A 4-bit value which defines the type of gate this Interrupt Descriptor represents. In long mode there are two valid type values:
        0b1110 or 0xE: 64-bit Interrupt Gate
        0b1111 or 0xF: 64-bit Trap Gate
        DPL: A 2-bit value which defines the CPU Privilege Levels which are allowed to access this interrupt via the INT instruction. Hardware interrupts ignore this mechanism.
        P: Present bit. Must be set (1) for the descriptor to be valid.
     */
    idt_desc64->offset_1 = (uint64_t)address & 0x0000ffff;
    idt_desc64->selector = KERNEL_CODE_SELECTOR;
    idt_desc64->ist0 = 0x00;
    idt_desc64->type_attributes = attribute;
    idt_desc64->offset_2 = (uint16_t)(((uint64_t)address) >> 16);
    idt_desc64->offset_3 = (uint32_t)(((uint64_t)address) >> 32);
}

void idt_handle_exception(int interrupt, struct interrupt_frame* frame)
{
    printk("%s: %d, error_code: %d", exception_messages[interrupt], interrupt, frame->error_code);
    if (frame->cs & RING3) process_exit();
    // while (1);
}

void interrupt_handler(int interrupt, struct interrupt_frame* frame)
{
    if (interrupt_callbacks[interrupt] != 0)
    {
        interrupt_callbacks[interrupt](interrupt, frame);
    }
    /* Acknowledge master PIC. */
    outb(0x20, 0x20);
}

void timer_handler(int interrupt, struct interrupt_frame* frame)
{
    task_wake_up(++current_ticks);
    outb(0x20, 0x20);
    if (task_list_next() != task_list_current())
    {
        yield();
    }
}

void idt_initialize()
{
    memset(&idt_descriptors64, 0, sizeof(idt_descriptors64));
    idtr_descriptor64.limit = sizeof(idt_descriptors64) - 1;
    idtr_descriptor64.base_address = (uint64_t)idt_descriptors64;
    // 0x8E => 0b10001110 Interrupt Gate, ring0 , idt valid
    // 0xEE => 0b11101110 Interrupt Gate, ring3 , idt valid
    for (int i = 0;i < TOTAL_INTERRUPTS; i++)
    {
        idt_set(i, interrupt_pointers[i], 0xEE);
    }

    idt_set(0x80, isr80h_wrapper, 0xEE);

    for (int i = 0; i < 0x20; i++)
    {
        idt_register_interrupt_callback(i, idt_handle_exception);
    }
    idt_register_interrupt_callback(0x20, timer_handler);
    load_idt(&idtr_descriptor64);
}

int idt_register_interrupt_callback(int interrupt, INTERRUPT_CALLBACK_FUNCTION interrupt_callback)
{
    int res = SUCCESS;
    if (interrupt < 0 || interrupt >= TOTAL_INTERRUPTS)
    {
        return -EINVARG;
    }
    interrupt_callbacks[interrupt] = interrupt_callback;
    return res;
}

void isr80h_register_command(int command_id, ISR80H_COMMAND command)
{
    if (command_id < 0 || command_id >= OS_MAX_ISR80H_COMMANDS)
    {
        printk("The command is out of range\n");
        assert(0);
    }
    if (isr80h_commands[command_id])
    {
        printk("The command already exists\n");
        assert(0);
    }
    assert(!isr80h_commands[command_id]);
    isr80h_commands[command_id] = command;
}

void* isr80h_handle_command(int command, struct interrupt_frame* frame)
{
    void* result = 0;
    if (command < 0 || command >= OS_MAX_ISR80H_COMMANDS)
    {
        return 0; // Invalid command
    }

    ISR80H_COMMAND command_func = isr80h_commands[command];
    if (!command_func)
    {
        return 0;
    }

    result = command_func(frame);
    return result;
}

void isr80h_handler(struct interrupt_frame* frame)
{
    // set_kernel_registers();
    int64_t command = frame->rdi;
    int64_t argc = frame->rsi;

    if (argc < 0 || command < 0) {
        frame->rax = 0;
        return;
    }
    frame->rax = (uint64_t)isr80h_handle_command(command, frame);
}
