#include "printk.h"
#include "assert.h"
#include "idt.h"
#include "kmemory.h"
#include "heap/kheap.h"
#include "paging/paging.h"

void kernel_main(void)
{
    terminal_screen_initialize();
    idt_initialize();
    kernel_heap_initialize();
    // get_memory_info();

    // void* p = kmalloc(5);
    // printk("%x\n", vir2phy((uint64_t)p));

    // void* p2 = kmalloc(4096 + 1);
    // printk("%x\n", vir2phy((uint64_t)p2));

    // void* p1 = kmalloc(5);
    // printk("%x\n", vir2phy((uint64_t)p1));

    // kfree(p2);
    // p2 = kmalloc(5);
    // printk("%x\n", vir2phy((uint64_t)p2));
    // p2 = kmalloc(5);
    // printk("%x\n", vir2phy((uint64_t)p2));
    // p2 = kmalloc(5);
    // printk("%x\n", vir2phy((uint64_t)p2));

    // p2 = kmalloc(1024 * 1024 * 1024);
    // printk("%x\n", vir2phy((uint64_t)p2));
    // enable_interrupts();

    struct pml4_table* pml4_table = paging_initialize(0b11);
    pml4_entry* e_ptr = pml4_table->entries;
    for (int i = 0; i < 512; i++)
    {
        if (i == 256)
            printk("%x\n", (uint64_t*)e_ptr);
        e_ptr++;
    }
    assert(0);
}