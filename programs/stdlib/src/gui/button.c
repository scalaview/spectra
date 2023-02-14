#include "gui/button.h"
#include "gui/gui.h"
#include "window/message.h"

bool button_default_procedure(struct gui_window* win, struct message* msg)
{
    if (msg->event == MESSAGE_MOUSE_PRESS)
    {
        struct gui_window* target = gui_window_get_root(win);
        if (!target) return false;
        msg->event = MESSAGE_CLOSE;
        return window_consume_message(target, msg);
    }
    return false;
}

button_struct* gui_window_create_button(struct gui_window* parent, uint32_t width, uint32_t height, int32_t x, int32_t y, int id, window_procedure procedure)
{
    button_struct* btn = create_gui_window(parent, width, height, x, y, id, 0);
    if (!btn) return 0;
    if (procedure)   btn->custom_procedure = procedure;
    btn->default_procedure = &button_default_procedure;
    return btn;
}
