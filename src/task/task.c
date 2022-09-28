#include "task.h"
#include "kmemory.h"
#include "heap/kheap.h"
#include "status.h"
#include "paging/paging.h"
#include "process.h"

struct tasks_manager tasks_manager = {
    .head = NULL,
    .current = NULL,
    .tail = NULL
};

struct task* task_list_current()
{
    return tasks_manager.current;
};

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
    // initialize stack and task memory space, 16KB stack size
    task->page_chunk = paging_initialize(
        (((uint64_t)process->program_info.virtual_base_address) - process->program_info.stack_size),
        ((uint64_t)process->program_info.virtual_end_address), vir2phy(program_memory),
        PAGE_SIZE_4K,
        PAGING_IS_WRITEABLE | PAGING_PRESENT | PAGING_ACCESS_FROM_ALL);
    if (!task->page_chunk)
    {
        res = -ENOMEM;
        goto out;
    }
    task->rptr = program_memory;
    task->tptr = (char*)program_memory + process->program_info.stack_size;
    task->registers.cs = USER_CODE_SEGMENT;
    task->registers.ss = USER_DATA_SEGMENT;
    task->registers.rsp = RANG_3_STACK_PTR;
    task->registers.rip = RANG_3_STACK_IP;
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
    task_switch(&task->registers);
}