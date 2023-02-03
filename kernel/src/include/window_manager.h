#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>

#include "config.h"
#include "message_queue.h"
#include "task.h"
#include "drivers/vga/vesa.h"

struct window
{
    uint32_t id;
    int x;
    int y;
    int z;
    struct screen_buffer* screen_buffer;
    int width;
    int height;
    bool need_draw;
    int message_queue_index;
    struct task* parent_task;
    struct message_queue message_queue;
};

struct window_wrapper
{
    struct window* win;
    struct window_wrapper* next;
    struct window_wrapper* prev;
};

int create_window(int x, int y, uint32_t width, uint32_t height, uint32_t gcolor, struct window** out_win);
void window_refresh();
#endif