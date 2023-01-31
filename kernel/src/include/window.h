#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>

#include "config.h"
#include "message_queue.h"
#include "task.h"
#include "drivers/vga/vesa.h"

struct window
{
    uint32_t handle;
    int x;
    int y;
    int z;
    struct screen_buffer* screen_buffer;
    int width;
    int height;
    int message_queue_index;
    struct task* parent_task;
    struct message_queue message_queue;
};

#endif