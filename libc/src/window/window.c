#include "window/window.h"

void putpixel(uint8_t* screen, int x, int y, uint32_t color, struct screen_buffer* screen_buffer) {
    unsigned where = x * screen_buffer->pixelwidth + y * screen_buffer->pitch;
    uint8_t alpha = (color >> 24) & 255;  // ALPHA
    if (!alpha)  return;
    screen[where] = color & 255;              // BLUE
    screen[where + 1] = (color >> 8) & 255;   // GREEN
    screen[where + 2] = (color >> 16) & 255;  // RED
}

void draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color, struct screen_buffer* screen_buffer) {
    for (int l = 0; l < height && (y + l) < screen_buffer->height; l++) {
        for (int i = 0; i < width && (x + i) < screen_buffer->width; i++) {
            putpixel(screen_buffer->buffer, x + i, y + l, color, screen_buffer);
        }
    }
}