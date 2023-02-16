#ifndef LIBC_WINDOW_H
#define LIBC_WINDOW_H

#include <stdint.h>
#include <stdbool.h>

#include "window/message.h"

#define TEXT_FONT_STATIC_WIDTH  8
#define TEXT_FONT_WIDTH(str) (strlen(str)*TEXT_FONT_STATIC_WIDTH)
#define TEXT_FONT_HEIGHT(str) (8)

#define POSITION_STABLE     0x00000001

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
    bool need_draw;
    int32_t x;
    int32_t y;
    int32_t z;
    uint16_t attributes;

    int id;
    uint32_t width;
    uint32_t height;
    int state;
    char* title;
    struct screen_buffer* buffer;

    window_procedure custom_procedure;
    window_procedure default_procedure;
    struct gui_window* dragged;

    struct gui_window* children;
    struct gui_window* other;
    struct gui_window* parent;
}__attribute__((packed));

typedef struct gui_window label_struct;
typedef struct gui_window button_struct;

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