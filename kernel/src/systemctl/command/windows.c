#include "systemctl/windows.h"
#include "window_manager.h"
#include "debug.h"

void* isr80h_command9_create_window_content(struct interrupt_frame* frame)
{
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
    create_window_content(x, y, width, height, gcolor, &win);
    return (void*)win;
}