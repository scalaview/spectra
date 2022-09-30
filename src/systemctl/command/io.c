#include "systemctl/io.h"
#include "printk.h"

void* isr80h_command0_print(struct interrupt_frame* frame)
{
    int64_t param_count = frame->rdi;
    int64_t* argptr = (int64_t*)frame->rsi;
    if (param_count < 2)
    {
        printk("missing params in print");
        return 0;
    }
    print_to_screen((char*)argptr[0], (int)argptr[1], 0xe);

    return (void*)argptr[1];
}
