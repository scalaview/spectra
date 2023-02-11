#include <stdint.h>

#include "window_manager.h"
#include "status.h"
#include "heap/kheap.h"
#include "kmemory.h"
#include "assert.h"
#include "config.h"
#include "process.h"
#include "message_queue.h"
#include "task.h"

struct window* head = 0;
struct window* tail = 0;
uint64_t __window_id = 0;
static struct window* focused_window;
extern struct video_info_struct vesa_video_info;

static void __window_list_remove_one(struct window* win)
{
    struct window* current = head->next;
    while (current)
    {
        if (current == win)
        {
            struct window* prev = win->prev;
            struct window* next = win->next;
            if (!prev)
            {
                assert(head == win);
                head = next;
            }
            else
            {
                prev->next = next;
            }
            if (!next)
            {
                assert(tail == win);
                tail = prev;
            }
            else
            {
                next->prev = prev;
            }
            win->prev = NULL;
            win->next = NULL;
            return;
        }
        current = current->next;
    }
    assert(0);
}

static void __append_window(struct window* win)
{
    if (win->next || win->prev) __window_list_remove_one(win);

    // sort window
    struct window* current = tail;
    while (current)
    {
        if (current->z <= win->z)
        {
            win->next = current->next;
            win->prev = current;
            current->next = win;
            break;
        }
        current = tail->prev;
    }
    // reach head
    if (!current)
    {
        win->next = head;
        if (head) head->prev = win;
        head = win;
    }

    if (!tail)
    {
        tail = win;
    }
}

int create_window_content(int x, int y, uint32_t width, uint32_t height, uint32_t gcolor, uint8_t* canvas, struct window_flags* flags, struct window** out_win)
{
    int res = 0;
    struct screen_buffer* screen_buffer;
    struct window* win;
    if (!canvas)
    {
        res = -EINVARG;
        goto out;
    }
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
    flags->handle = win_id;
    flags->need_draw = false;
    win->flags = flags;
    win->parent_task = task_list_current();


    __append_window(win);
    screen_buffer = (struct screen_buffer*)kzalloc(sizeof(struct screen_buffer));
    if (!screen_buffer)
    {
        res = -ENOMEM;
        goto out;
    }
    screen_buffer->height = height;
    screen_buffer->width = width;
    screen_buffer->pixelwidth = vesa_video_info.pixelwidth;
    screen_buffer->pitch = width * screen_buffer->pixelwidth;
    screen_buffer->canvas = canvas;

    if (!screen_buffer->canvas)
    {
        res = -ENOMEM;
        goto out;
    }
    screen_buffer->pixelsize = screen_buffer->width * screen_buffer->height * screen_buffer->pixelwidth;
    memset(screen_buffer->canvas, gcolor, screen_buffer->pixelsize);

    win->screen_buffer = screen_buffer;
    memset(win->message_queue.buffer, 0, sizeof(struct message) * OS_MAX_MESSAGE_LENGTH);
    *out_win = win;
    focused_window = win;
out:
    if (res && screen_buffer) kfree(screen_buffer);
    if (res && win) kfree(win);

    return res;
}

void window_free(struct window* window)
{
    if (window->flags) kfree(window->flags);
    if (window->screen_buffer && window->screen_buffer->canvas) kfree(window->screen_buffer->canvas);
    if (window->screen_buffer) kfree(window->screen_buffer);
    kfree(window);
}

void window_copy_rect(struct window* src)
{
    uint8_t pixelwidth = vesa_video_info.pixelwidth;
    int64_t reserve_pixel = src->x > 0 ? (src->x * pixelwidth) : 0;
    int64_t hidden_pixel = src->x > 0 ? 0 : -(src->x * pixelwidth);
    uint32_t pixel_len = calculate_pixel_len(src->x, src->width, vesa_video_info.width) * pixelwidth;

    for (int i = 0; i < src->height; i++)
    {
        void* start_line_addr = (void*)(((uint64_t)vesa_video_info.buffer) + (src->y + i) * vesa_video_info.width * pixelwidth + reserve_pixel);
        void* start_buffer_addr = (void*)(((uint64_t)src->screen_buffer->canvas) + i * src->screen_buffer->width * pixelwidth + hidden_pixel);
        memcpy(start_line_addr, start_buffer_addr, pixel_len);
    }
}

void __window_flush_screen_buffer()
{
    memcpy((void*)vesa_video_info.vir_linear_addr, vesa_video_info.buffer, vesa_video_info.width * vesa_video_info.height * vesa_video_info.pixelwidth);
}

void window_refresh()
{
    struct window* current = head;
    while (current)
    {
        if (current->flags->need_draw) window_copy_rect(current);
        current = current->next;
    }

    draw_cursor();
    __window_flush_screen_buffer();
}

void window_add_message(struct window* win, struct message* msg)
{
    message_push(&win->message_queue, msg);
}

void window_add_message_to_focused(struct message* msg)
{
    if (focused_window == NULL) {
        return;
    }
    window_add_message(focused_window, msg);
}

void window_pop_message(struct window* win, struct message* msg_out)
{
    message_pop(&win->message_queue, msg_out);
}

struct window* window_fetch(uint32_t id)
{
    struct window* current = head;
    while (current)
    {
        if (current->id == id) return current;
        current = current->next;
    }
    return 0;
}