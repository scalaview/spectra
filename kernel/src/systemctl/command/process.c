#include "systemctl/process.h"
#include "printk.h"
#include "task.h"

void* isr80h_command1_sleep(struct interrupt_frame* frame)
{
    int64_t argc = frame->rsi;
    int64_t* argv = (int64_t*)frame->rdx;
    if (argc < 1)
    {
        printk("missing params in print");
        return 0;
    }
    uint64_t current = get_current_ticks();
    uint64_t pass = current;
    uint64_t sleep_seconds = argv[0];
    // uint64_t end = current + sleep_seconds * 10;
    task_sleep_current(sleep_seconds * 10);
    while (current - pass < sleep_seconds) {
        current = get_current_ticks();
    }

    return 0;
}
