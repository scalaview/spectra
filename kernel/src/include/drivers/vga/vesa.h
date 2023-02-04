#ifndef VESA_DRIVER_H
#define VESA_DRIVER_H
#include <stdint.h>
#include "config.h"
#include "paging/paging.h"
#include "window/window.h"

#define TEXT_FONT_WIDTH(str) (strlen(str)*8)
#define TEXT_FONT_HEIGHT(str) (8)

struct video_info_struct {
    unsigned char* buffer;
    uint32_t width;
    uint32_t height;
    uint8_t pixelwidth;
    uint32_t pitch;
    uint32_t pixelsize;
    uint64_t linear_addr;
    uint64_t vir_linear_addr;
    int bits;
    uint8_t  type;
}__attribute__((packed));

struct vga_font {
    char* font;
    unsigned width;
    unsigned height;
};

void vga_setfont(const struct vga_font* font);

void kernel_init_vesa();
int map_vesa_paging(struct pml4_table* pml4_table);
uint64_t calculate_pixel_len(int x, uint64_t width, uint64_t screen_width);
void gfx_puts(int x, int y, uint32_t fgcolor, uint32_t bgcolor, const char* c, struct screen_buffer* screen_buffer);
extern void putpixel(uint8_t* screen, int x, int y, uint32_t color, uint32_t pitch, uint32_t pixelwidth);
extern void draw_cursor();

#endif