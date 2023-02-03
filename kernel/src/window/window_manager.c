#include <stdint.h>

#include "window_manager.h"
#include "status.h"
#include "heap/kheap.h"
#include "kmemory.h"
#include "assert.h"
#include "config.h"

struct window_wrapper* head = 0;
struct window_wrapper* tail = 0;
uint64_t __window_id = 0;

static void __window_list_remove_one(struct window_wrapper* window_wrapper)
{
    struct window_wrapper* current = head->next;
    while (current)
    {
        if (current == window_wrapper)
        {
            struct window_wrapper* prev = window_wrapper->prev;
            struct window_wrapper* next = window_wrapper->next;
            if (!prev)
            {
                assert(head == window_wrapper);
                head = next;
            }
            else
            {
                prev->next = next;
            }
            if (!next)
            {
                assert(tail == window_wrapper);
                tail = prev;
            }
            else
            {
                next->prev = prev;
            }
            window_wrapper->prev = NULL;
            window_wrapper->next = NULL;
            return;
        }
        current = current->next;
    }
    assert(0);
}
static void __window_list_insert(struct window_wrapper* window_wrapper)
{
    if (window_wrapper->next || window_wrapper->prev) __window_list_remove_one(window_wrapper);

    // sort window
    struct window_wrapper* current = tail;
    while (current)
    {
        if (current->win->z <= window_wrapper->win->z)
        {
            window_wrapper->next = current->next;
            window_wrapper->prev = current;
            current->next = window_wrapper;
            break;
        }
        current = tail->prev;
    }
    // reach head
    if (!current)
    {
        window_wrapper->next = head;
        if (head) head->prev = window_wrapper;
        head = window_wrapper;
    }

    if (!tail)
    {
        tail = window_wrapper;
    }

}

static int __append_window(struct window* win)
{
    struct window_wrapper* window_wrapper = (struct window_wrapper*)kzalloc(sizeof(struct window_wrapper));
    if (!window_wrapper) return -ENOMEM;

    window_wrapper->win = win;
    __window_list_insert(window_wrapper);
    return 0;
}


int create_window(int x, int y, uint32_t width, uint32_t height, uint32_t gcolor, struct window** out_win)
{
    int res = 0;
    struct screen_buffer* screen_buffer;
    struct window* win;
    int win_id = __window_id++;
    if (win_id < 0)
    {
        res = win_id;
        goto out;
    }

    win = (struct window*)kzalloc(sizeof(struct window));
    if (!win)
    {
        res = -ENOMEM;
        goto out;
    }

    win->height = height;
    win->width = width;
    win->x = x;
    win->y = y;
    win->z = 0;
    win->id = win_id;
    win->need_draw = false;

    __append_window(win);
    screen_buffer = (struct screen_buffer*)kzalloc(sizeof(struct screen_buffer));
    if (!screen_buffer)
    {
        res = -ENOMEM;
        goto out;
    }
    extern struct video_info_struct vesa_video_info;
    screen_buffer->height = height;
    screen_buffer->width = width;
    screen_buffer->pitch = vesa_video_info.pitch;
    screen_buffer->pixelwidth = vesa_video_info.pixelwidth;
    screen_buffer->buffer = (uint8_t*)kzalloc(screen_buffer->width * screen_buffer->height * screen_buffer->pixelwidth);

    if (!screen_buffer->buffer)
    {
        res = -ENOMEM;
        goto out;
    }
    screen_buffer->pixelsize = screen_buffer->width * screen_buffer->height * screen_buffer->pixelwidth;

    win->screen_buffer = screen_buffer;
    memset(win->message_queue.buffer, 0, sizeof(struct message) * OS_MAX_MESSAGE_LENGTH);
    *out_win = win;

out:
    if (res && screen_buffer) kfree(screen_buffer);
    if (res && win) kfree(win);

    return res;
}

void window_copy_rect(struct window* src)
{
    extern struct video_info_struct vesa_video_info;
    uint8_t pixelwidth = vesa_video_info.pixelwidth;
    int64_t reserve_pixel = src->x > 0 ? (src->x * pixelwidth) : 0;
    int64_t hidden_pixel = src->x > 0 ? 0 : -(src->x * pixelwidth);
    uint32_t pixel_len = calculate_pixel_len(src->x, src->width, vesa_video_info.width) * pixelwidth;

    for (int i = 0; i < src->height; i++)
    {
        void* start_line_addr = (void*)(vesa_video_info.buffer + i * vesa_video_info.width * pixelwidth + reserve_pixel);
        void* start_buffer_addr = src->screen_buffer->buffer + i * src->screen_buffer->width * pixelwidth + hidden_pixel;
        memcpy(start_line_addr, start_buffer_addr, pixel_len);
    }
}

void __window_flush_screen_buffer()
{
    extern struct video_info_struct vesa_video_info;
    memcpy((void*)vesa_video_info.vir_linear_addr, vesa_video_info.buffer, vesa_video_info.width * vesa_video_info.height * vesa_video_info.pixelwidth);
}

void window_refresh()
{
    struct window_wrapper* current = head;
    while (current)
    {
        if (current->win->need_draw) window_copy_rect(current->win);
        current = current->next;
    }

    draw_cursor();
    __window_flush_screen_buffer();
}