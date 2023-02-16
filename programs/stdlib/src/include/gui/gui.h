#ifndef OS_STD_GUI_H
#define OS_STD_GUI_H

#include <stdint.h>
#include "window/window.h"

#define GUI_CONTROL_PANEL_CLOSS_BUTTON_ID 99999


#define GUI_CONTROL_PANEL_HEIGHT 24
#define GUI_CONTROL_PANEL_CLOSE_BTN_HEIGHT 16
#define GUI_CONTROL_PANEL_CLOSE_BTN_WIDTH 16


#define WINDOW_CLOSE    2

extern struct gui_window* create_window_content(int32_t x, int32_t y, int32_t z, uint32_t width, uint32_t height, uint32_t gcolor, uint16_t attributes);
struct gui_window* create_gui_window(struct gui_window* parent, uint32_t width, uint32_t height, int32_t x, int32_t y, int32_t z, int id, const char* title, uint16_t attributes);
label_struct* create_window_control_panel(struct gui_window* win, int id);
void screan_putchar(struct screen_buffer* buffer, const char ch, uint32_t* current_width, uint32_t* current_height, uint32_t t_color, uint32_t tb_color, uint32_t reserved);
bool window_consume_message(struct gui_window* win, struct message* msg);
bool window_consume_message_simple(struct gui_window* win, uint16_t event);
bool window_consume(struct gui_window* win, struct message* msg);
button_struct* gui_window_create_close_button(struct gui_window* win, int id);
struct gui_window* gui_window_get_root(struct gui_window* win);
extern void sys_window_free(int id);
void gui_window_free(struct gui_window* win);
void draw_rect_in_absolute_position(struct gui_window* parent, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);

#endif