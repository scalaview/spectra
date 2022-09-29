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
#include "tss.h"
#include "process.h"

extern struct pml4_table* kernel_chunk;

void kernel_main(uint32_t magic, struct multiboot_info* mbi_phya)
{

    terminal_screen_initialize();
    idt_initialize();

    initialize_tss();

    kernel_heap_initialize();
    // get_memory_info();

    kernel_chunk = kernel_paging_initialize();
    // disable to access low address
    setup_paging_directory(vir2phy(kernel_chunk->entries));

    fs_initialize();
    disk_search_and_initialize();
    // enable_interrupts();

    // struct path_root* ipath = path_parse("0:/boot/kernel.elf");
    // printk("%d", ipath->drive_no);
    // void* ptr = kzalloc(100);
    // printk("%x\n", vir2phy(ptr));
    // kfree(ptr);
    // FILE* fd = fopen("0:/data/ext2", "r");
    // assert(fd->fdi);
    // struct file_stat* stat = kzalloc(sizeof(struct file_stat));
    // fstat(fd->fdi, stat);
    // char* str = kzalloc(stat->filesize);
    // assert(str);
    // fseek(fd, stat->filesize - 100, SEEK_SET);
    // fread(str, 100, 1, fd);
    // printk("read from 0:/data/ext2: %s", str);
    // fclose(fd);
    // kfree(fd);
    // kfree(str);
    // ptr = kzalloc(100);
    // printk("%x\n", vir2phy(ptr));
    // kfree(ptr);

    struct process* process = 0;
    if (process_initialize(&process))
    {
        printk("init process fail!");
        assert(0);
    }
    process_launch(process->id);
    assert(0);
}