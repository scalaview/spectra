#ifndef LIBC_WINDOW_H
#define LIBC_WINDOW_H

#include <stdint.h>
#include <stdbool.h>

#include "window/message.h"

#define TEXT_FONT_WIDTH(str) (strlen(str)*8)
#define TEXT_FONT_HEIGHT(str) (8)

// canvas
struct screen_buffer
{
    uint8_t* canvas;
    uint32_t width;
    uint32_t height;
    uint8_t pixelwidth;
    uint32_t pitch;
    uint32_t pixelsize;
}__attribute__((packed));


typedef uint64_t window_handle;
struct gui_window;
typedef bool(*window_procedure)(struct gui_window*, struct message*);

struct gui_window
{
    window_handle handle;
    int id;
    int x;
    int y;
    int width;
    int height;
    int state;
    char* title;

    window_procedure window_procedure;
    window_procedure default_procedure;

    struct gui_window* next;
    struct gui_window* parent;
};

struct vga_font
{
    char* font;
    unsigned width;
    unsigned height;
};

void vga_setfont(const struct vga_font* f);
void putpixel(uint8_t* screen, int x, int y, uint32_t color, uint32_t pitch, uint32_t pixelwidth);
void draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color, struct screen_buffer* screen_buffer);
void gfx_puts(int x, int y, uint32_t fgcolor, uint32_t bgcolor, const char* c, struct screen_buffer* screen_buffer);

#endif