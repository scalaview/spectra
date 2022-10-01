#include "task.h"
#include "kmemory.h"
#include "heap/kheap.h"
#include "status.h"
#include "paging/paging.h"
#include "process.h"
#include "assert.h"

struct tasks_manager tasks_manager = {
    .head = NULL,
    .current = NULL,
    .tail = NULL
};

struct task* task_list_current()
{
    return tasks_manager.current;
};

static void task_save_state(struct task* task, struct interrupt_frame* frame)
{
    task->registers->rax = frame->rax;
    task->registers->rbx = frame->rbx;
    task->registers->rcx = frame->rcx;
    task->registers->rdx = frame->rdx;
    task->registers->rsi = frame->rsi;
    task->registers->rdi = frame->rdi;
    task->registers->rbp = frame->rbp;
    task->registers->r8 = frame->r8;
    task->registers->r9 = frame->r9;
    task->registers->r10 = frame->r10;
    task->registers->r11 = frame->r11;
    task->registers->r12 = frame->r12;
    task->registers->r13 = frame->r13;
    task->registers->r14 = frame->r14;
    task->registers->r15 = frame->r15;

    task->registers->rip = frame->rip;
    task->registers->cs = frame->cs;
    task->registers->rflags = frame->rflags;
    task->registers->rsp = frame->rsp;
    task->registers->ss = frame->ss;
}

void task_save_current_state(struct interrupt_frame* frame)
{
    struct task* task = task_list_current();
    assert(task);
    task_save_state(task, frame);
}

int task_initialize(struct task* task, struct process* process)
{
    int res = 0;
    memset(task, 0, sizeof(struct task));
    void* program_memory = kzalloc(process->program_info.stack_size + process->program_info.size);
    if (!program_memory)
    {
        res = -ENOMEM;
        goto out;
    }
    void* kernel_stack = kzalloc(4 * PAGE_SIZE_4K);
    if (!kernel_stack)
    {
        res = -ENOMEM;
        goto out;
    }
    // map kernel page
    task->page_chunk = kernel_paging_initialize();
    if (!task->page_chunk)
    {
        res = -ENOMEM;
        goto out;
    }
    // initialize stack and task memory space, 16KB stack size
    paging_initialize_pml4_table(&task->page_chunk, (((uint64_t)process->program_info.virtual_base_address) - process->program_info.stack_size), ((uint64_t)process->program_info.virtual_end_address), vir2phy(program_memory), PAGE_SIZE_4K, PAGING_IS_WRITEABLE | PAGING_PRESENT | PAGING_ACCESS_FROM_ALL);
    task->tstack_top = program_memory;
    task->entry = (void*)(((char*)program_memory) + process->program_info.stack_size);
    task->kstack = (void*)(((char*)kernel_stack) + 4 * PAGE_SIZE_4K);
    task->registers = (struct registers*)program_memory;
    task->registers->cs = USER_CODE_SEGMENT | 3;
    task->registers->ss = USER_DATA_SEGMENT | 3;
    task->registers->rsp = RANG_3_STACK_PTR;
    task->registers->rip = RANG_3_VMA;
    task->registers->rflags = 0x202; // enable interrupt
    task->process = process;

out:
    if (res < 0)
    {
        kfree(program_memory);
    }
    return res;
}

struct task* create_task(struct process* process)
{
    int res = 0;
    struct task* task = kzalloc(sizeof(struct task));
    if (!task)
    {
        res = -ENOMEM;
        goto out;
    }
    task_initialize(task, process);
out:
    if (res);
    return task;
}

void task_launch(struct task* task)
{
    tasks_manager.current = task;
    task_switch(task->registers);
}