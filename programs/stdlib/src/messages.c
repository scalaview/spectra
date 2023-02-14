#include "messages.h"

void window_get_message(struct gui_window* win, struct message* msg)
{
    sys_window_get_message(win->handle, msg);
}