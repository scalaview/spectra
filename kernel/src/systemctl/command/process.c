#include "systemctl/process.h"
#include "debug.h"
#include "task.h"
#include "process.h"

void* isr80h_command1_sleep(struct interrupt_frame* frame)
{
    int64_t argc = frame->rsi;
    int64_t* argv = (int64_t*)frame->rdx;
    if (argc < 1)
    {
        debug_printf("missing params in sleep");
        return 0;
    }
    uint64_t current = get_current_ticks();
    uint64_t sleep_seconds = argv[0];
    task_sleep_until(current + sleep_seconds);
    return 0;
}

void* isr80h_command2_exit(struct interrupt_frame* frame)
{
    process_exit();
    return 0;
}

void* isr80h_command3_wait(struct interrupt_frame* frame)
{
    int64_t argc = frame->rsi;
    int64_t* argv = (int64_t*)frame->rdx;
    if (argc < 1)
    {
        debug_printf("missing params in wait");
        return 0;
    }
    int32_t pid = argv[0];
    return (void*)((int64_t)process_wait(pid));
}

void* isr80h_command4_fork(struct interrupt_frame* frame)
{
    return (void*)((int64_t)process_fork());
}

void* isr80h_command5_execve(struct interrupt_frame* frame)
{
    int64_t argc = frame->rsi;
    int64_t* argv = (int64_t*)frame->rdx;
    if (argc < 4)
    {
        debug_printf("missing params in execve");
        return 0;
    }
    const char* pathname = (char*)argv[0];
    const char** cargv = (const char**)argv[1];
    const char** envp = (const char**)argv[2];
    RING_LEV ring_lev = argv[3];
    return (void*)((int64_t)process_execve(pathname, cargv, envp, ring_lev));
}

void* isr80h_command6_malloc(struct interrupt_frame* frame)
{
    int64_t argc = frame->rsi;
    int64_t* argv = (int64_t*)frame->rdx;
    if (argc < 1)
    {
        debug_printf("missing params in malloc");
        return 0;
    }
    size_t size = (size_t)argv[0];
    return process_malloc(size);
}

void* isr80h_command7_free(struct interrupt_frame* frame)
{
    int64_t argc = frame->rsi;
    int64_t* argv = (int64_t*)frame->rdx;
    if (argc < 1)
    {
        debug_printf("missing params in free");
        return 0;
    }
    void* ptr = (void*)argv[0];
    process_malloc_free(ptr);
    return 0;
}