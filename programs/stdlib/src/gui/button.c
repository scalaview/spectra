#include "gui/button.h"
#include "gui/gui.h"
#include "window/message.h"
#include "stdio.h"

bool button_default_procedure(struct gui_window* win, struct message* msg)
{
    return false;
}

button_struct* gui_window_create_button(struct gui_window* parent, uint32_t width, uint32_t height, int32_t x, int32_t y, int32_t z, int id, uint16_t attributes, window_procedure procedure)
{
    button_struct* btn = create_gui_window(parent, width, height, x, y, z, id, 0, attributes);
    if (!btn) return 0;
    if (procedure)   btn->custom_procedure = procedure;
    btn->default_procedure = &button_default_procedure;
    return btn;
}
