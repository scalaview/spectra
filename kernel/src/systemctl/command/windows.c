#include "systemctl/windows.h"
#include "window_manager.h"
#include "debug.h"
#include "window/window.h"
#include "kmemory.h"

void* isr80h_command9_create_window_content(struct interrupt_frame* frame)
{
    extern struct video_info_struct vesa_video_info;
    int64_t argc = frame->rsi;
    int64_t* argv = (int64_t*)frame->rdx;
    if (argc < 5)
    {
        debug_printf("missing params in create_window");
        return 0;
    }
    int x = argv[0];
    int y = argv[1];
    uint32_t width = argv[2];
    uint32_t height = argv[3];
    uint32_t gcolor = argv[4];

    struct window* win = 0;
    struct allocation* buffer_allocation = process_alloc(width * height * vesa_video_info.pixelwidth);
    if (!buffer_allocation)
    {
        debug_printf("create_window fail!, not enought memory");
        return 0;
    }

    struct allocation* window_flags_allocation = process_alloc(sizeof(struct gui_window));
    if (!window_flags_allocation)
    {
        debug_printf("create_window fail!, not enought memory");
        return 0;
    }

    struct screen_buffer* buffer = (struct screen_buffer*)process_malloc(sizeof(struct screen_buffer));
    if (!buffer)
    {
        debug_printf("create_window fail!, not enought memory");
        return 0;
    }

    int res = create_window_content(x, y, width, height, gcolor, (uint8_t*)buffer_allocation->kptr, (struct window_flags*)window_flags_allocation->kptr, &win);
    if (res)
    {
        debug_printf("create_window fail!, error code: %d", res);
        return 0;
    }

    memcpy(buffer, win->screen_buffer, sizeof(struct screen_buffer));
    buffer->canvas = (uint8_t*)buffer_allocation->tptr;
    struct gui_window* gui_window = (struct gui_window*)window_flags_allocation->tptr;
    gui_window->buffer = buffer;
    gui_window->height = height;
    gui_window->width = width;
    gui_window->x = x;
    gui_window->y = y;

    return (void*)gui_window;
}