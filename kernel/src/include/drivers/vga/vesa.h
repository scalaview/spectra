#ifndef VESA_DRIVER_H
#define VESA_DRIVER_H
#include <stdint.h>
#include "config.h"
#include "paging/paging.h"
#include "assets/img/tga.h"

struct video_info_struct {
    unsigned char* buffer;
    uint32_t width;
    uint32_t height;
    uint8_t pixelwidth;
    int pitch;
    int pixelsize;
    uint64_t linear_addr;
    uint64_t vir_linear_addr;
    int bits;
    uint8_t  type;
}__attribute__((packed));

struct screen_buffer
{
    uint8_t* buffer;
    uint32_t width;
    uint32_t height;
    uint8_t pixelwidth;
    int pitch;
    int pixelsize;
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
extern void putpixel(uint8_t* screen, int x, int y, uint32_t color, struct screen_buffer* screen_buffer);
extern void draw_cursor();

#endif