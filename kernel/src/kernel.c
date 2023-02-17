#include "printk.h"
#include "assert.h"
#include "debug.h"
#include "idt.h"
#include "kmemory.h"
#include "heap/kheap.h"
#include "paging/paging.h"
#include "multiboot/multiboot.h"
#include "path.h"
#include "file.h"
#include "disk.h"
#include "tss.h"
#include "process.h"
#include "task.h"
#include "isr80h.h"
#include "string.h"
#include "drivers/keyboard/keyboard.h"
#include "drivers/vga/vesa.h"
#include "drivers/mouse/mouse.h"
#include "window_manager.h"
#include "window/window.h"
#include "assets/img/tga.h"

extern struct pml4_table* kernel_chunk;
void test_draw();
struct tga_content* background_pic = 0;

void kernel_main(uint32_t magic, struct multiboot_info* mbi_phya)
{
    terminal_screen_initialize();
    idt_initialize();

    initialize_tss();

    kernel_heap_initialize();
    // get_memory_info();
    void* p = kzalloc(1);
    debug_printf("before kernel paging remap %x\n", p);
    kfree(p);

    kernel_chunk = kernel_paging_initialize();
    assert(kernel_chunk);
    p = kzalloc(1);
    debug_printf("after kernel paging remap %x\n", p);
    kfree(p);

    debug_printf("kernel_chunk %x\n", kernel_chunk);

    // disable to access low address
    setup_paging_directory(vir2phy(kernel_chunk->entries));

    kernel_init_vesa();

    fs_initialize();
    disk_search_and_initialize();
    // enable_interrupts();

    isr80h_register_commands();
    keyboard_initialize();

    mouse_initialize();

    debug_printf("hello word!");

    struct pml4_table* pm4 = 0;
    paging_initialize_pml4_table(&pm4, KERNEL_VMA, KERNEL_VM_MAX, KERNEL_PHY_BASE, PAGE_SIZE_2M, PAGING_IS_WRITEABLE | PAGING_PRESENT);
    free_paging(pm4);
    pm4 = 0;
    paging_initialize_pml4_table(&pm4, KERNEL_VMA, KERNEL_VM_MAX, KERNEL_PHY_BASE, PAGE_SIZE_2M, PAGING_IS_WRITEABLE | PAGING_PRESENT);
    paging_initialize_pml4_table(&pm4, 0x400000, 0x800000, 0x2000000, PAGE_SIZE_4K, PAGING_IS_WRITEABLE | PAGING_PRESENT | PAGING_ACCESS_FROM_ALL);
    free_paging(pm4);

    struct path_root* ipath = path_parse("0:/boot/kernel.elf");
    debug_printf("%d", ipath->drive_no);
    void* ptr = kzalloc(100);
    debug_printf("%x\n", ptr);
    kfree(ptr);
    FILE* fd = fopen("0:/data/ext2", "r");
    assert(fd->fdi);
    struct file_stat* stat = kzalloc(sizeof(struct file_stat));
    fstat(fd->fdi, stat);
    char* str = kzalloc(stat->filesize);
    assert(str);
    fseek(fd, stat->filesize - 100, SEEK_SET);
    fread(str, 100, 1, fd);
    debug_printf("read from 0:/data/ext2: %s", str);
    fclose(fd);
    kfree(fd);
    kfree(str);
    ptr = kzalloc(100);
    debug_printf("%x\n", ptr);
    kfree(ptr);

    init_idle_process();
    struct process* init = 0;
    if (create_kernel_process("0:/usr/bin/init.bin", &init) < 0)
    {
        debug_printf("init process fail!");
        assert(0);
    }
    process_launch(init->id);
}