#include "task/none.h"
#include "task.h"
#include "process.h"

void __process_allocation_free(struct process* process, void* task_address);

struct allocation* none_malloc(struct task* task, size_t size)
{
    return task_malloc(task, size);
}

void none_free(struct process* process, void* ptr)
{
    if (!ptr) return;
    __process_allocation_free(process, ptr);
}