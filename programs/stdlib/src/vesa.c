#include "driver/vga/vesa.h"
#include "driver/vga/font.h"
#include "window/window.h"

struct vga_font font;

struct vga_font font8x8_basic = {
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

