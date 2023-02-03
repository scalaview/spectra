#include "drivers/vga/vesa.h"
#include "drivers/vga/font.h"
#include "drivers/mouse/mouse.h"
#include "multiboot/multiboot_info.h"
#include "assert.h"
#include "kmemory.h"
#include "heap/kheap.h"
#include "file.h"
#include "assert.h"
#include "debug.h"

struct video_info_struct vesa_video_info;
static struct vga_font font;

struct tga_content* cursor = 0;

static struct vga_font font8x8_basic = {
    .font = font_pixel_8x8,
    .height = 8,
    .width = 8,
};

void vga_setfont(const struct vga_font* f)
{
    font.font = f->font;
    font.height = f->height;
    font.width = f->width;
}

void gfx_putchar(int x, int y, uint32_t fgcolor, uint32_t bgcolor, const char c, struct screen_buffer* screen_buffer) {
    uint8_t i, j;
    uint32_t t = (c & 127) * font.width;
    for (i = 0; i < font.width; i++) {
        for (j = 0; j < font.height; j++) {
            if (x + i >= 0 && x + 1 < screen_buffer->width &&
                y + j >= 0 && y + 1 < screen_buffer->height) {
                putpixel(screen_buffer->buffer, x + i, y + j, ((font.font[t + j] >> i) & 1) ? fgcolor : bgcolor, screen_buffer);
            }
        }
    }
}

void gfx_puts(int x, int y, uint32_t fgcolor, uint32_t bgcolor, const char* c, struct screen_buffer* screen_buffer) {
    while (*c) {
        gfx_putchar(x, y, fgcolor, bgcolor, *c++, screen_buffer);
        x += 8;
    }
}

void extract_multiboot_framebuffer_tag()
{
    extern uint32_t multiboot_magic;
    extern struct multiboot_info* multiboot_info;
    struct multiboot_info* info = (struct multiboot_info*)phy2vir(multiboot_info);
    assert(multiboot_magic == MULTIBOOT2_BOOTLOADER_MAGIC);

    struct multiboot_tag* tag;
    for (tag = (struct multiboot_tag*)info->tags;
        tag->type != MULTIBOOT_TAG_TYPE_END;
        tag = (struct multiboot_tag*)((multiboot_uint8_t*)tag
            + ((tag->size + 7) & ~7)))
    {
        if (tag->type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER) {
            struct multiboot_tag_framebuffer* tagfb = (struct multiboot_tag_framebuffer*)tag;
            void* fb = (void*)(unsigned long)tagfb->common.framebuffer_addr;

            vesa_video_info.linear_addr = (uint64_t)fb;
            vesa_video_info.width = tagfb->common.framebuffer_width;
            vesa_video_info.height = tagfb->common.framebuffer_height;
            vesa_video_info.bits = tagfb->common.framebuffer_bpp;
            vesa_video_info.pitch = tagfb->common.framebuffer_pitch;
            vesa_video_info.type = tagfb->common.framebuffer_type;
            vesa_video_info.pixelwidth = vesa_video_info.bits / 8;
            vesa_video_info.pixelsize = vesa_video_info.width * vesa_video_info.height * vesa_video_info.pixelwidth;
            vesa_video_info.vir_linear_addr = KERNEL_VGA_BASE;
            // Initialize screen buffer
            vesa_video_info.buffer = (unsigned char*)kzalloc(vesa_video_info.pixelsize);
            if (!vesa_video_info.buffer) assert(0);
            return;
        }
    }
    assert(0);
}

int map_vesa_paging(struct pml4_table* pml4_table)
{
    return paging_initialize_pml4_table(&pml4_table, vesa_video_info.vir_linear_addr,
        align_up_2m(vesa_video_info.vir_linear_addr + vesa_video_info.pixelsize),
        vesa_video_info.linear_addr, PAGE_SIZE_2M, PAGING_IS_WRITEABLE | PAGING_PRESENT);
}

void kernel_init_vesa()
{
    extract_multiboot_framebuffer_tag();
    extern struct pml4_table* kernel_chunk;
    map_vesa_paging(kernel_chunk);
    vga_setfont(&font8x8_basic);
}

void draw_icon(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t pixels[], struct screen_buffer* screen_buffer) {
    for (int l = 0; l < h && (y + l) < screen_buffer->height; l++) {
        for (int i = 0; i < w && (x + i) < screen_buffer->width; i++) {
            int64_t position = l * w + i;
            putpixel(screen_buffer->buffer, x + i, y + l, pixels[position], screen_buffer);
        }
    }
}

void draw_transparent_icon(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t pixels[], struct screen_buffer* screen_buffer) {
    for (int l = 0; l < h && (y + l) < screen_buffer->height; l++) {
        for (int i = 0; i < w && (x + i) < screen_buffer->width; i++) {
            int64_t position = l * w + i;
            // check alpha value 
            if (pixels[position] ^ 0xff000000) putpixel(screen_buffer->buffer, x + i, y + l, pixels[position], screen_buffer);
        }
    }
}


uint64_t calculate_pixel_len(int x, uint64_t width, uint64_t screen_width)
{
    int64_t display_width = x + width;
    if (x >= 0)
    {
        return display_width > screen_width ? (screen_width - x) : width;
    }
    else
    {
        return display_width > 0 ? display_width : 0;
    }
}

void draw_cursor()
{
    if (!cursor)
    {
        FILE* fd = fopen("0:/data/cursor_16.tga", "r");
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
        cursor = tga_parse(pngptr, stat->filesize);
    }
    draw_transparent_icon(mouse_x, mouse_y, cursor->width, cursor->height, cursor->pixels, (struct screen_buffer*)&vesa_video_info);
}
