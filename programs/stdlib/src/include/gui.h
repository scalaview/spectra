#ifndef OS_STD_GUI_H
#define OS_STD_GUI_H

#include <stdint.h>
#include "window/window.h"

#define GUI_CONTROL_PANEL_HEIGHT 20

extern struct gui_window* create_window_content(int64_t x, int64_t y, uint32_t width, uint32_t height, uint32_t gcolor);
label_struct* create_window_lable(struct gui_window* parent, uint32_t width, uint32_t height, int64_t x, int64_t y, int id, const char* title, uint32_t color, uint32_t t_color, uint32_t tb_color);
struct gui_window* create_gui_window(struct gui_window* parent, uint32_t width, uint32_t height, int64_t x, int64_t y, int id, const char* title);
label_struct* create_window_control_panel(struct gui_window* win, int id);
void screan_putchar(struct screen_buffer* buffer, const char ch, uint32_t* current_width, uint32_t* current_height, uint32_t t_color, uint32_t tb_color, uint32_t reserved);
bool window_consume_message(struct gui_window* win, struct message* msg);
bool window_consume_message_simple(struct gui_window* win, uint16_t event);
void window_consume(struct gui_window* win, struct message* msg);

#endif