#include "printk.h"
#include "assert.h"
#include "idt.h"
#include "kmemory.h"
#include "heap/kheap.h"
#include "paging/paging.h"
#include "multiboot.h"
#include "path.h"
#include "file.h"
#include "disk.h"

extern struct pml4_table* kernel_chunk;

void kernel_main(uint32_t magic, struct multiboot_info* mbi_phya)
{
    terminal_screen_initialize();
    idt_initialize();

    kernel_heap_initialize();
    get_memory_info();

    kernel_chunk = kernel_paging_initialize();
    // disable to access low address
    setup_paging_directory((pml4_entry*)vir2phy(kernel_chunk->entries));

    fs_initialize();
    disk_search_and_initialize();
    // enable_interrupts();

    // struct path_root* ipath = path_parse("0:/boot/kernel.elf");
    // printk("%d", ipath->drive_no);
    FILE* fd = fopen("0:/doc/Making_a_GRUB2_image.md", "r");
    if (fd);

    assert(0);
}