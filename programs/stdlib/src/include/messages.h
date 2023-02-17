#ifndef OS_STD_MESSAGE_H
#define OS_STD_MESSAGE_H

#include <stddef.h>
#include "window/window.h"

extern void sys_window_push_message(uint32_t id, struct message* message);
extern void sys_window_get_message(uint32_t id, struct message* message);
void window_get_message(struct gui_window* win, struct message* msg);
void window_push_message(struct gui_window* win, struct message* msg);
#endif