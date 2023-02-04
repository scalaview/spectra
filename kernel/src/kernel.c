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
    test_draw();
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


void read_background()
{
    if (!background_pic)
    {
        FILE* fd = fopen("0:/data/background.tga", "r");
        assert(fd->fdi);
        struct file_stat* stat = kzalloc(sizeof(struct file_stat));
        fstat(fd->fdi, stat);
        unsigned char* pngptr = kzalloc(stat->filesize);
        assert(pngptr);
        fread(pngptr, stat->filesize, 1, fd);
        fclose(fd);
        kfree(stat);

        struct tga_header* tga_header = (struct tga_header*)pngptr;
        if (tga_header);
        background_pic = tga_parse(pngptr, stat->filesize);
    }
}

void draw_background()
{
    extern struct video_info_struct vesa_video_info;
    struct window* background = 0;
    int res = create_window_content(0, 0, vesa_video_info.width, vesa_video_info.height, 0, &background);
    assert(!res);
    read_background();
    assert(background_pic);
    memcpy(background->screen_buffer->buffer, background_pic->pixels, background->screen_buffer->pixelsize);
    background->need_draw = true;
}

void test_draw_window()
{
    extern struct video_info_struct vesa_video_info;
    struct window* win = 0;
    int res = create_window_content(100, 100, 500, 500, 0xffffffff, &win);
    assert(!res);
    draw_rect(0, 0, win->width, 20, 0xff000000, win->screen_buffer);
    char* c = "hello word!";
    debug_printf("x: %d", (win->width - TEXT_FONT_WIDTH(c)) / 2);
    debug_printf("y: %d", (20 - TEXT_FONT_HEIGHT(c)) / 2);
    gfx_puts((win->width - TEXT_FONT_WIDTH(c)) / 2, (20 - TEXT_FONT_HEIGHT(c)) / 2, 0xFFFFFFFF, 0x0, c, win->screen_buffer);
    win->need_draw = true;
}

void test_draw()
{
    draw_background();
    test_draw_window();
}