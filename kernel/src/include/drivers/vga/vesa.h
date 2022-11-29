#ifndef VESA_DRIVER_H
#define VESA_DRIVER_H
#include <stdint.h>
#include "config.h"
#include "paging/paging.h"
#include "assets/img/tga.h"

struct video_info_struct {
    uint64_t linear_addr;
    unsigned char* buffer;
    int width;
    int height;
    int bits;
    int pitch;
    uint8_t  type;
};

struct vga_font {
    char* font;
    unsigned width;
    unsigned height;
};

void vga_setfont(const struct vga_font* font);

#define vga_phy2vir(p) KERNEL_VGA_BASE

void kernel_init_vesa();
int map_vesa_paging(struct pml4_table* pml4_table);
void test_draw();
void test_draw1();
#endif