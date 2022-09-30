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
    // map kernel page
    task->page_chunk = kernel_paging_initialize();
    if (!task->page_chunk)
    {
        res = -ENOMEM;
        goto out;
    }
    // initialize stack and task memory space, 16KB stack size
    paging_initialize_pml4_table(&task->page_chunk, (((uint64_t)process->program_info.virtual_base_address) - process->program_info.stack_size), ((uint64_t)process->program_info.virtual_end_address), vir2phy(program_memory), PAGE_SIZE_4K, PAGING_IS_WRITEABLE | PAGING_PRESENT | PAGING_ACCESS_FROM_ALL);
    task->rptr = program_memory;
    task->tptr = (char*)program_memory + process->program_info.stack_size;
    task->registers = (struct registers*)program_memory;
    task->registers->r12 = 100;
    task->registers->cs = USER_CODE_SEGMENT | 3;
    task->registers->ss = USER_DATA_SEGMENT | 3;
    task->registers->rsp = RANG_3_STACK_PTR;
    task->registers->rip = RANG_3_VMA;
    task->registers->rflags = 0x202;
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
    task_switch(task->registers);
}