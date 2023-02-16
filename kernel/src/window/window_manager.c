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
#include "drivers/mouse/mouse.h"
#include "debug.h"

extern struct video_info_struct vesa_video_info;

struct window* head = 0;
struct window* tail = 0;
static uint64_t __window_id = 0;
static int32_t __max_z = 0;
static uint16_t __previous_message_key = 0;

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
        if (current->container->z <= win->container->z)
        {
            win->next = current->next;
            win->prev = current;
            current->next = win;
            if (current == tail)
            {
                tail = win;
            }
            break;
        }
        current = current->prev;
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

int create_window_content(int32_t x, int32_t y, int32_t z, uint32_t width, uint32_t height, uint32_t gcolor, uint8_t* canvas, uint16_t attributes, struct window_container* container, struct window** out_win)
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

    win->id = win_id;
    container->handle = win_id;
    container->need_draw = false;
    container->x = x;
    container->y = y;
    container->z = z;
    container->attributes |= attributes;
    if (!(attributes & POSITION_STABLE))
        container->z = __max_z++;
    win->container = container;
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
    win->message_queue.head = 0;
    win->message_queue.tail = 0;
    *out_win = win;

out:
    if (res && screen_buffer) kfree(screen_buffer);
    if (res && win) kfree(win);

    return res;
}

void window_free(struct window* window)
{
    __window_list_remove_one(window);
    if (window->screen_buffer && window->screen_buffer->canvas) kfree(window->screen_buffer->canvas);
    if (window->screen_buffer) kfree(window->screen_buffer);
    kfree(window);
}

void window_copy_rect(struct window* src)
{
    uint8_t pixelwidth = vesa_video_info.pixelwidth;
    int64_t reserve_pixel = src->container->x > 0 ? (src->container->x * pixelwidth) : 0;
    int64_t hidden_pixel = src->container->x > 0 ? 0 : -(src->container->x * pixelwidth);
    uint32_t pixel_len = calculate_pixel_len(src->container->x, src->width, vesa_video_info.width) * pixelwidth;

    for (int i = (src->container->y >= 0) ? 0 : -src->container->y;
        i < src->height && (src->container->y + i) < vesa_video_info.height; i++)
    {
        void* start_line_addr = (void*)(((uint64_t)vesa_video_info.buffer) + (src->container->y + i) * vesa_video_info.width * pixelwidth + reserve_pixel);
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
    int32_t z = 0;
    while (current)
    {
        if (current->container->need_draw)
        {
            window_copy_rect(current);
        }
        if (!(current->container->attributes & POSITION_STABLE))
        {
            current->container->z = z++;
        }
        current = current->next;
    }
    __max_z = z;
    draw_cursor();
    __window_flush_screen_buffer();
}

void window_add_message(struct window* win, struct message* msg)
{
    message_push(win->parent_task, &win->message_queue, msg);
}

void window_add_message_to_focused(struct message* msg)
{
    if (tail == NULL) {
        return;
    }
    window_add_message(tail, msg);
}

void window_pop_message(struct window* win, struct message* msg_out)
{
    message_pop(win->parent_task, &win->message_queue, msg_out);
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

bool window_belongs_to(struct window* win, int16_t x, int16_t y)
{
    return x >= win->container->x && x <= win->container->x + win->width && y >= win->container->y && y <= win->container->y + win->height;
}

struct window* window_find_absolue_position(int16_t x, int16_t y)
{
    if (!tail) {
        return 0;
    }
    struct window* current = tail;
    while (current)
    {
        if (window_belongs_to(current, mouse_x, mouse_y))
        {
            return current;
        }
        current = current->prev;
    }
    return 0;
}

void window_change_focused(int32_t key)
{
    extern int16_t mouse_x;
    extern int16_t mouse_y;
    if (key & MOUSE_LEFT_CLICK)
    {
        struct window* win = window_find_absolue_position(mouse_x, mouse_y);

        if (win && !(win->container->attributes & POSITION_STABLE) && win != tail)
        {
            tail->container->z = __max_z++;
            __window_list_remove_one(win);
            win->container->z = __max_z++;
            __append_window(win);
        }
    }
}

static void __window_relative_position(struct window* win, int16_t x, int16_t y, int16_t* out_x, int16_t* out_y)
{
    *out_x = x - win->container->x;
    *out_y = y - win->container->y;
}

void window_handle_message(struct message* msg)
{
    // Don't change focused window when mouse draging
    if (!(__previous_message_key & MOUSE_LEFT_DRAG) || !(msg->key & MOUSE_LEFT_DRAG))  window_change_focused(msg->key);

    if (msg->event)
    {
        // relative position
        int16_t x = 0;
        int16_t y = 0;
        __window_relative_position(tail, msg->x, msg->y, &x, &y);
        debug_printf("after window_relative_position x%d, y%d, x:%d, y:%d\n", msg->x, msg->y, x, y);
        msg->x = x;
        msg->y = y;
        window_add_message_to_focused(msg);
    }
    __previous_message_key = msg->key;
}
