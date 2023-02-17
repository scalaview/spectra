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
    if (argc < 6)
    {
        debug_printf("missing params in create_window");
        return 0;
    }
    int x = argv[0];
    int y = argv[1];
    int z = argv[2];
    uint32_t width = argv[3];
    uint32_t height = argv[4];
    uint32_t gcolor = argv[5];
    uint32_t attributes = argv[6];

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

    int res = create_window_content(x, y, z, width, height, gcolor, (uint8_t*)buffer_allocation->kptr, attributes, (struct window_container*)window_flags_allocation->kptr, &win);
    if (res)
    {
        debug_printf("create_window fail!, error code: %d", res);
        return 0;
    }

    memcpy(buffer, win->screen_buffer, sizeof(struct screen_buffer));
    buffer->canvas = (uint8_t*)buffer_allocation->tptr;
    struct gui_window* gui_window = (struct gui_window*)window_flags_allocation->tptr;
    gui_window->buffer = buffer;

    return (void*)gui_window;
}

void* isr80h_command11_free_window_content(struct interrupt_frame* frame)
{
    int64_t argc = frame->rsi;
    int64_t* argv = (int64_t*)frame->rdx;
    if (argc < 1)
    {
        debug_printf("missing params in free window");
        return 0;
    }

    int id = argv[0];
    struct window* win = window_fetch(id);
    if (!win)
    {
        debug_printf("no window found");
        return 0;
    }

    window_free(win);
    return 0;
}