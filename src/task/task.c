#include "task.h"
#include "kmemory.h"
#include "heap/kheap.h"
#include "status.h"
#include "paging/paging.h"

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
    memset(task, 0, sizeof(struct task));
    task->page_chunk = paging_initialize((RANG_3_VMA - 4 * PAGE_SIZE_4K), RANG_3_VM_MAX, RANG_3_PHY_BASE, PAGE_SIZE_4K, PAGING_IS_WRITEABLE | PAGING_PRESENT | PAGING_ACCESS_FROM_ALL);
    if (!task->page_chunk)
    {
        return -ENOMEM;
    }
    task->registers.cs = USER_CODE_SEGMENT;
    task->registers.ss = USER_DATA_SEGMENT;
    task->registers.rsp = RANG_3_STACK_PTR;
    task->process = process;
    return 0;
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