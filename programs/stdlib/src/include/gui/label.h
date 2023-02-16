#ifndef OS_STD_LABEL_H
#define OS_STD_LABEL_H

#include <stdint.h>
#include <stddef.h>
#include "gui/gui.h"

label_struct* create_window_label(struct gui_window* parent, uint32_t width, uint32_t height, int32_t x, int32_t y, int32_t z, int id, const char* title, uint16_t attributes, uint32_t color, uint32_t t_color, uint32_t tb_color);

#endif