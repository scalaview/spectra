#include "systemctl/process.h"
#include "debug.h"
#include "task.h"
#include "process.h"
#include "window_manager.h"

void* isr80h_command10_get_message(struct interrupt_frame* frame)
{
    int64_t argc = frame->rsi;
    int64_t* argv = (int64_t*)frame->rdx;
    if (argc < 2)
    {
        debug_printf("missing params in get message");
        return 0;
    }
    uint32_t id = argv[0];
    struct message* msg = (struct message*)argv[1];
    struct window* win = window_fetch(id);
    if (win)
    {
        window_pop_message(win, msg);
    }
    return (void*)0;
}
