#ifndef OS_STD_GUI_H
#define OS_STD_GUI_H

#include <stdint.h>
#include "window/window.h"

#define GUI_CONTROL_PANEL_CLOSS_BUTTON_ID 99999


#define GUI_CONTROL_PANEL_HEIGHT 30
#define GUI_CONTROL_PANEL_CLOSE_BTN_HEIGHT 24
#define GUI_CONTROL_PANEL_CLOSE_BTN_WIDTH 24

#define WINDOW_CLOSE    2

typedef void(*draw_function)(struct gui_window* win, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t pixels[]);
typedef int(*pixels_producteur)(struct gui_window* win, uint32_t x, uint32_t y, uint32_t width, uint32_t height, draw_function draw_function);

extern struct gui_window* create_window_content(int32_t x, int32_t y, int32_t z, uint32_t width, uint32_t height, uint32_t gcolor, uint16_t attributes);
struct gui_window* create_gui_window(struct gui_window* parent, uint32_t width, uint32_t height, int32_t x, int32_t y, int32_t z, int id, const char* title, uint16_t attributes, pixels_producteur producteur);
label_struct* create_window_control_panel(struct gui_window* win, int id);
bool window_consume_message(struct gui_window* win, struct message* msg);
bool window_consume_message_simple(struct gui_window* win, uint16_t event);
bool window_consume(struct gui_window* win, struct message* msg);
int draw_with_file(struct gui_window* win, uint32_t x, uint32_t y, uint32_t width, uint32_t height, const char* filepath);
button_struct* gui_window_create_close_button(struct gui_window* win, int id, pixels_producteur producteur);
struct gui_window* gui_window_get_root(struct gui_window* win);
extern void sys_window_free(int id);
void gui_window_free(struct gui_window* win);
void draw_rect_in_absolute_position(struct gui_window* parent, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
void gui_window_print(struct gui_window* win, int32_t x, int32_t y, const char* str, uint32_t t_color, uint32_t tb_color, uint32_t reserved);
void screan_putchar(struct screen_buffer* buffer, const char cha, int32_t* current_x, int32_t* current_y, uint32_t t_color, uint32_t tb_color, uint32_t reserved);
void draw_with_pixels(struct gui_window* win, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t pixels[]);

#endif