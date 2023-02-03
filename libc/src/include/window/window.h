#ifndef LIBC_WINDOW_H
#define LIBC_WINDOW_H

#include <stdint.h>

struct screen_buffer
{
    uint8_t* buffer;
    uint32_t width;
    uint32_t height;
    uint8_t pixelwidth;
    int pitch;
    int pixelsize;
}__attribute__((packed));

void putpixel(uint8_t* screen, int x, int y, uint32_t color, struct screen_buffer* screen_buffer);
void draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color, struct screen_buffer* screen_buffer);

#endif