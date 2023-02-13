#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>

#include "config.h"
#include "message_queue.h"
#include "task.h"
#include "drivers/vga/vesa.h"

struct window_flags
{
    uint64_t handle;
    bool need_draw;
}__attribute__((packed));

struct window
{
    uint32_t id;
    int z;
    struct screen_buffer* screen_buffer;
    int width;
    int height;
    struct window_flags* flags;
    bool keep_z_stale;
    struct task* parent_task;
    struct message_queue message_queue;
    struct window* next;
    struct window* prev;
};

int create_window_content(int x, int y, uint32_t width, uint32_t height, uint32_t gcolor, uint8_t* canvas, struct window_flags* flags, struct window** out_win);
void window_free(struct window* window);
void window_refresh();
void window_add_message(struct window* win, struct message* msg);
void window_add_message_to_focused(struct message* msg);
void window_pop_message(struct window* win, struct message* msg_out);
struct window* window_fetch(uint32_t id);
bool window_belongs_to(struct window* win, int16_t x, int16_t y);
struct window* window_find_absolue_position(int16_t x, int16_t y);
void window_change_focused(int32_t key);
void window_handle_message(struct message* msg);

#endif