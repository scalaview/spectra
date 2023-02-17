#ifndef OS_STD_BUTTON_H
#define OS_STD_BUTTON_H

#include <stdint.h>
#include <stddef.h>
#include "gui/gui.h"

button_struct* gui_window_create_button(struct gui_window* parent, uint32_t width, uint32_t height, int32_t x, int32_t y, int32_t z, int id, uint16_t attributes, window_procedure procedure, pixels_producteur producteur);

#endif