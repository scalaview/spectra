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
    uint64_t sleep_seconds = argv[0];
    task_sleep_until(current + sleep_seconds);
    return 0;
}
