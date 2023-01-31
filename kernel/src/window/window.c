#include <stdint.h>

#include "window.h"
#include "status.h"
#include "heap/kheap.h"


int create_window(int x, int y, uint32_t width, uint32_t height, uint32_t gcolor, struct window** out_win)
{
    int res = 0;
    struct screen_buffer* screen_buffer;
    struct window* win = (struct window*)kzalloc(sizeof(struct window));
    if (!win)
    {
        res = -ENOMEM;
        goto out;
    }

    win->height = height;
    win->width = width;
    win->x = x;
    win->y = y;

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
    win->screen_buffer = screen_buffer;
    *out_win = win;

out:
    if (screen_buffer) kfree(screen_buffer);
    if (win) kfree(win);

    return res;
}