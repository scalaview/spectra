#include "driver/vga/vesa.h"

void setup_env()
{
    extern struct vga_font font8x8_basic;
    vga_setfont(&font8x8_basic);
    return;
}