#include "window/window.h"

extern struct vga_font font;

void putpixel(uint8_t* screen, int x, int y, uint32_t color, uint32_t pitch, uint32_t pixelwidth)
{
    unsigned where = x * pixelwidth + y * pitch;
    uint8_t alpha = (color >> 24) & 255;  // ALPHA
    if (!alpha)  return;
    screen[where] = color & 255;              // BLUE
    screen[where + 1] = (color >> 8) & 255;   // GREEN
    screen[where + 2] = (color >> 16) & 255;  // RED
    screen[where + 3] = alpha;  // ALPHA
}

void draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color, struct screen_buffer* screen_buffer)
{
    for (int l = 0; l < height && (y + l) < screen_buffer->height; l++) {
        for (int i = 0; i < width && (x + i) < screen_buffer->width; i++) {
            putpixel(screen_buffer->canvas, x + i, y + l, color, screen_buffer->pitch, screen_buffer->pixelwidth);
        }
    }
}

static void __gfx_putchar(int x, int y, uint32_t fgcolor, uint32_t bgcolor, const char c, struct screen_buffer* screen_buffer)
{
    uint8_t i, j;
    uint32_t t = (c & 127) * font.width;
    for (i = 0; i < font.width; i++) {
        for (j = 0; j < font.height; j++) {
            if (x + i >= 0 && x + 1 < screen_buffer->width &&
                y + j >= 0 && y + 1 < screen_buffer->height) {
                putpixel(screen_buffer->canvas, x + i, y + j, ((font.font[t + j] >> i) & 1) ? fgcolor : bgcolor, screen_buffer->pitch, screen_buffer->pixelwidth);
            }
        }
    }
}

void gfx_puts(int x, int y, uint32_t fgcolor, uint32_t bgcolor, const char* c, struct screen_buffer* screen_buffer)
{
    if (!font.font)
    {
        extern struct vga_font font8x8_basic;
        vga_setfont(&font8x8_basic);
    }
    while (*c) {
        __gfx_putchar(x, y, fgcolor, bgcolor, *c++, screen_buffer);
        x += 8;
    }
}

void draw_transparent_icon(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t pixels[], struct screen_buffer* screen_buffer)
{
    for (int l = 0; l < h && (y + l) < screen_buffer->height; l++) {
        for (int i = 0; i < w && (x + i) < screen_buffer->width; i++) {
            int64_t position = l * w + i;
            // check alpha value 
            if (pixels[position] ^ 0xff000000) putpixel(screen_buffer->canvas, x + i, y + l, pixels[position], screen_buffer->pitch, screen_buffer->pixelwidth);
        }
    }
}