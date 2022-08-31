#include "printk.h"
#include "assert.h"
#include "idt.h"
#include "memory.h"

extern void divide_zero();

void kernel_main(void)
{
    terminal_screen_initialize();
    idt_initialize();
    get_memory_info();
    enable_interrupts();
    char* str = "hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\n";
    int64_t value = 0x123456789ABCD;
    for (int i = 0;i < 25; i++)
        printk("%d, %s", i, str);

    printk("This value is % \nequal to %d", value);
    assert(0);
}