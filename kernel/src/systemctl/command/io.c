#include "systemctl/io.h"
#include "debug.h"
#include "drivers/keyboard/keyboard.h"
#include "file.h"
#include "process.h"
#include "task.h"
#include "kmemory.h"

extern void _debug_putchars(const char* buffer, int size, char color);

void* isr80h_command0_print(struct interrupt_frame* frame)
{
    int64_t argc = frame->rsi;
    int64_t* argv = (int64_t*)frame->rdx;
    if (argc < 2)
    {
        debug_printf("missing params in print");
        return 0;
    }
    _debug_putchars((char*)argv[0], (int)argv[1], (char)argv[2]);

    return (void*)argv[1];
}


void* isr80h_command8_readkey(struct interrupt_frame* frame)
{
    char c = keyboard_pop();
    return (void*)((int64_t)c);
}

void* isr80h_command12_fopen(struct interrupt_frame* frame)
{
    int64_t argc = frame->rsi;
    int64_t* argv = (int64_t*)frame->rdx;
    if (argc < 2)
    {
        debug_printf("missing params in fopen");
        return 0;
    }
    char* filename = (char*)argv[0];
    char* mode = (char*)argv[1];

    FILE* fd = fopen(filename, mode);
    struct allocation* fd_allocation = process_alloc(sizeof(FILE));
    FILE* fd_out = (FILE*)fd_allocation->kptr;
    memcpy(fd_out, fd, sizeof(FILE));
    return fd_allocation->tptr;
}

void* isr80h_command13_fstat(struct interrupt_frame* frame)
{
    int64_t argc = frame->rsi;
    int64_t* argv = (int64_t*)frame->rdx;
    if (argc < 2)
    {
        debug_printf("missing params in fstat");
        return 0;
    }

    int fd = argv[0];
    struct allocation* stat_allocation = process_fetch_allocation(task_list_current()->process, (void*)argv[1]);

    struct file_stat* stat = (struct file_stat*)stat_allocation->kptr;
    fstat(fd, stat);
    return 0;
}

void* isr80h_command14_fread(struct interrupt_frame* frame)
{
    int64_t argc = frame->rsi;
    int64_t* argv = (int64_t*)frame->rdx;
    if (argc < 4)
    {
        debug_printf("missing params in fread");
        return 0;
    }
    struct allocation* buff_allocation = process_fetch_allocation(task_list_current()->process, (void*)argv[0]);
    struct allocation* fd_allocation = process_fetch_allocation(task_list_current()->process, (void*)argv[3]);
    if (!buff_allocation || !fd_allocation)
    {
        debug_printf("memory not found");
        return 0;
    }

    return (void*)fread(buff_allocation->kptr, (uint32_t)argv[1], (uint32_t)argv[2], (FILE*)fd_allocation->kptr);
}


void* isr80h_command15_fclose(struct interrupt_frame* frame)
{
    int64_t argc = frame->rsi;
    int64_t* argv = (int64_t*)frame->rdx;
    if (argc < 1)
    {
        debug_printf("missing params in close");
        return 0;
    }

    FILE* fd = (FILE*)argv[0];

    fclose(fd);

    return 0;
}

void* isr80h_command16_fseek(struct interrupt_frame* frame)
{
    int64_t argc = frame->rsi;
    int64_t* argv = (int64_t*)frame->rdx;
    if (argc < 3)
    {
        debug_printf("missing params in close");
        return 0;
    }

    int64_t size = fseek((FILE*)argv[0], argv[1], (FILE_SEEK_MODE)argv[2]);
    return (void*)size;
}