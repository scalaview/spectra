#ifndef VESA_DRIVER_H
#define VESA_DRIVER_H
#include <stdint.h>
#include "config.h"
#include "paging/paging.h"

struct video_info_struct {
    uint64_t linear_addr;
    uint64_t addr;
    int width;
    int height;
    int bits;
    int pitch;
    uint8_t  type;
};

struct tga_header {
    unsigned char magic1;             // must be zero
    unsigned char colormap;           // must be zero
    unsigned char encoding;           // must be 2
    unsigned short cmaporig, cmaplen; // must be zero
    unsigned char cmapent;            // must be zero
    unsigned short x;                 // must be zero
    unsigned short y;                 // image's height
    unsigned short h;                 // image's height
    unsigned short w;                 // image's width
    unsigned char bpp;                // must be 32
    unsigned char pixeltype;          // must be 40
} __attribute__((packed));

struct tga_content {
    uint32_t width;
    uint32_t height;
    uint32_t pixels[];
};

// KERNEL_VGA_BASE
#define vga_phy2vir(p) ((uint64_t)(p) + KERNEL_VGA_BASE)

void kernel_init_vesa();
int map_vesa_paging(struct pml4_table* pml4_table);
void test_draw();

#endif