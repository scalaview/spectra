#include "idt.h"
#include "config.h"
#include "kmemory.h"
#include "assert.h"
#include "printk.h"
#include "io.h"

struct idt_desc64 idt_descriptors64[TOTAL_INTERRUPTS];
struct idtr_desc64 idtr_descriptor64;

extern void load_idt(struct idtr_desc64* ptr);
extern void* interrupt_pointers[TOTAL_INTERRUPTS];

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

void no_interrupt()
{
    /* Acknowledge master PIC. */
    outb(0x20, 0x20);
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
    idt_set(46, no_interrupt, 0xEE);
    load_idt(&idtr_descriptor64);
}

void interrupt_handler(int interrupt_no, struct interrupt_frame* frame)
{
    printk("interrupt_no: %d, frame error_code: %d", interrupt_no, frame->error_code);
    /* Acknowledge master PIC. */
    outb(0x20, 0x20);
    switch (interrupt_no)
    {
    case 32: // timer
        break;
    default:
        while (1);
    }
}