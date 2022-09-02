#include "printk.h"
#include "assert.h"
#include "idt.h"
#include "memory.h"

void kernel_main(void)
{
    terminal_screen_initialize();
    idt_initialize();

    get_memory_info();
    // enable_interrupts();
    assert(0);
}