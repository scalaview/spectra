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
#include "task.h"
#include "isr80h.h"

extern struct pml4_table* kernel_chunk;

void kernel_main(uint32_t magic, struct multiboot_info* mbi_phya)
{

    terminal_screen_initialize();
    idt_initialize();

    initialize_tss();

    kernel_heap_initialize();
    // get_memory_info();
    void* p = kzalloc(1);
    printk("before kernel paging remap %x\n", vir2phy(p));
    kfree(p);

    kernel_chunk = kernel_paging_initialize();
    assert(kernel_chunk);
    p = kzalloc(1);
    printk("after kernel paging remap %x\n", vir2phy(p));
    kfree(p);

    printk("kernel_chunk %x\n", vir2phy(kernel_chunk));

    // disable to access low address
    setup_paging_directory(vir2phy(kernel_chunk->entries));

    fs_initialize();
    disk_search_and_initialize();
    // enable_interrupts();

    isr80h_register_commands();

    struct pml4_table* pm4 = 0;
    paging_initialize_pml4_table(&pm4, KERNEL_VMA, KERNEL_VM_MAX, KERNEL_PHY_BASE, PAGE_SIZE_2M, PAGING_IS_WRITEABLE | PAGING_PRESENT);
    free_paging(pm4);
    pm4 = 0;
    paging_initialize_pml4_table(&pm4, KERNEL_VMA, KERNEL_VM_MAX, KERNEL_PHY_BASE, PAGE_SIZE_2M, PAGING_IS_WRITEABLE | PAGING_PRESENT);
    paging_initialize_pml4_table(&pm4, 0x400000, 0x800000, 0x2000000, PAGE_SIZE_4K, PAGING_IS_WRITEABLE | PAGING_PRESENT | PAGING_ACCESS_FROM_ALL);
    free_paging(pm4);

    struct path_root* ipath = path_parse("0:/boot/kernel.elf");
    printk("%d", ipath->drive_no);
    void* ptr = kzalloc(100);
    printk("%x\n", vir2phy(ptr));
    kfree(ptr);
    FILE* fd = fopen("0:/data/ext2", "r");
    assert(fd->fdi);
    struct file_stat* stat = kzalloc(sizeof(struct file_stat));
    fstat(fd->fdi, stat);
    char* str = kzalloc(stat->filesize);
    assert(str);
    fseek(fd, stat->filesize - 100, SEEK_SET);
    fread(str, 100, 1, fd);
    printk("read from 0:/data/ext2: %s", str);
    fclose(fd);
    kfree(fd);
    kfree(str);
    ptr = kzalloc(100);
    printk("%x\n", vir2phy(ptr));
    kfree(ptr);

    struct process* process = 0;
    if (process_initialize("0:/usr/bin/start.bin", &process) < 0)
    {
        printk("init process fail!");
        assert(0);
    }
    struct task* task = 0;
    process_initialize_task(process, &task);
    task_launch(task);
    process_launch(process->id);
    assert(0);
}