#include "printk.h"
#include "assert.h"
#include "idt.h"
#include "memory.h"

void kernel_main(void)
{
    idt_initialize();
    terminal_screen_initialize();
    get_memory_info();
    // enable_interrupts();
    assert(0);
}