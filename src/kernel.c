#include "printk.h"
#include "assert.h"
#include "idt.h"
#include "kmemory.h"
#include "heap/kheap.h"
#include "paging/paging.h"
#include "multiboot.h"

extern struct pml4_table* kernel_chunk;

void kernel_main(uint32_t magic, struct multiboot_info* mbi_phys)
{
    terminal_screen_initialize();
    idt_initialize();
    init_memory_map(magic, mbi_phys);
    // printk("hello");
    // kernel_heap_initialize();
    // kernel_chunk = kernel_paging_initialize();
    // // disable to access low address
    // setup_paging_directory((pml4_entry*)vir2phy(kernel_chunk->entries));
    // // get_memory_info();

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

    assert(0);
}