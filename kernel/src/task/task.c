#include "task.h"
#include "kmemory.h"
#include "heap/kheap.h"
#include "status.h"
#include "paging/paging.h"
#include "process.h"
#include "assert.h"
#include "tss.h"
#include "printk.h"

struct tasks_manager tasks_manager = {
    .current = NULL
};

struct task* task_list_current()
{
    return tasks_manager.current;
}

struct task* task_list_next()
{
    return tasks_manager.ready_list.next;
}

void task_list_set_current(struct task* task)
{
    task->prev = NULL;
    task->next = NULL;
    tasks_manager.current = task;
}

struct task* task_list_pop_head(struct task_wrapper* list)
{
    struct task* task_head = list->next;
    task_list_remove_one(list, task_head);
    return task_head;
}

bool is_list_empty(struct task_wrapper* list)
{
    return (list->next == NULL);
}

void task_ready_list_append_one(struct task* task)
{
    task->state = TASK_READY;
    task_list_add_one(&tasks_manager.ready_list, task);
}

void task_list_add_one(struct task_wrapper* list, struct task* task)
{
    assert(!task->prev);
    assert(!task->next);
    if (!list->next)
    {
        list->next = task;
    }
    else
    {
        list->tail->next = task;
        task->prev = list->tail;
    }
    list->tail = task;
};

void task_list_remove_one(struct task_wrapper* list, struct task* task)
{
    struct task* current = list->next;
    while (current)
    {
        if (current == task)
        {
            struct task* prev = task->prev;
            struct task* next = task->next;
            if (!prev)
            {
                assert(list->next == task);
                list->next = next;
            }
            else
            {
                prev->next = next;
            }
            if (!next)
            {
                assert(list->tail == task);
                list->tail = prev;
            }
            else
            {
                next->prev = prev;
            }
            task->prev = NULL;
            task->next = NULL;
            break;
        }
        current = current->next;
    }
}

static void task_initialize_stack(struct task* task, struct process* process)
{
    task->registers = (struct registers*)((uint64_t)task->k_stack - sizeof(struct registers));

    // adjust task_context_switch function
    task->k_context = (uint64_t)task->registers - 7 * 8;
    // set return address at restore_registers function
    *(uint64_t*)(task->k_context + 6 * 8) = (uint64_t)restore_registers;
    task->registers->cs = USER_CODE_SEGMENT | 3;
    task->registers->ss = USER_DATA_SEGMENT | 3;
    task->registers->rsp = RANG_3_STACK_PTR;
    task->registers->rip = RANG_3_VMA;
    task->registers->rflags = 0x202; // enable interrupt
}

void* task_stack_bottom(void* stack, size_t size)
{
    return (((char*)stack) - size);
}

int task_initialize(struct task* task, struct process* process)
{
    int res = 0;
    void* task_stack;
    void* kernel_stack;
    memset(task, 0, sizeof(struct task));
    kernel_stack = kzalloc(4 * PAGE_SIZE_4K);
    if (!kernel_stack)
    {
        res = -ENOMEM;
        goto out;
    }
    task_stack = kzalloc(process->program_info.stack_size);
    if (!task_stack)
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
    // initialize task memory space.
    paging_initialize_pml4_table(&task->page_chunk, (uint64_t)process->program_info.virtual_base_address, (uint64_t)process->program_info.virtual_end_address, vir2phy(process->program_info.ptr), PAGE_SIZE_4K, PAGING_IS_WRITEABLE | PAGING_PRESENT | PAGING_ACCESS_FROM_ALL);

    // initialize 16KB stack size
    paging_initialize_pml4_table(&task->page_chunk, (uint64_t)process->program_info.virtual_base_address - process->program_info.stack_size, (uint64_t)process->program_info.virtual_base_address, vir2phy(task_stack), PAGE_SIZE_4K, PAGING_IS_WRITEABLE | PAGING_PRESENT | PAGING_ACCESS_FROM_ALL);

    task->k_stack = (void*)(((char*)kernel_stack) + 4 * PAGE_SIZE_4K);
    task->t_stack = (void*)(((char*)task_stack) + process->program_info.stack_size);

    task_initialize_stack(task, process);
    task->state = TASK_WAIT;
    task->process = process;
    if (process->primary)
    {
        struct task* tptr = process->primary;
        if (tptr->th_next)
        {
            task->th_next = tptr->th_next;
        }
        tptr->th_next = task; // link treads
    }
    task_list_add_one(&tasks_manager.wait_list, task); //push to wait list

out:
    if (res < 0)
    {
        kfree(kernel_stack);
        kfree(task_stack);
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
    task->state = TASK_INIT;
    res = task_initialize(task, process);
out:
    if (res < 0)
    {
        kfree(task);
        return 0;
    }
    return task;
}

void switch_vm(struct pml4_table* pml4_table)
{
    setup_paging_directory(vir2phy(pml4_table->entries));
}

void task_launch(struct task* task)
{
    task_list_set_current(task);
    set_tss_rsp0((uint64_t)task->k_stack);
    switch_vm(task->page_chunk);
    set_user_registers();
    task_start(task->registers);
}

void task_switch(struct task* next)
{
    struct task* current = tasks_manager.current;
    task_list_set_current(next);
    set_tss_rsp0((uint64_t)next->k_stack);
    switch_vm(next->page_chunk);
    set_user_registers();
    task_context_switch(&current->k_context, next->k_context);
}

struct task* task_remove_ready_list_head()
{
    return task_list_pop_head(&tasks_manager.ready_list);
}

void task_schedule()
{
    if (is_list_empty(&tasks_manager.ready_list))
    {
        assert(0);
    }
    struct task* task_head = task_remove_ready_list_head();
    task_head->state = TASK_RUNNING;
    task_switch(task_head);
}

void tasks_run()
{
    if (is_list_empty(&tasks_manager.ready_list))
    {
        return;
    }
    struct task* task_head = task_remove_ready_list_head();
    task_head->state = TASK_RUNNING;
    task_launch(task_head);
}

void yield()
{
    if (is_list_empty(&tasks_manager.ready_list))
    {
        return;
    }
    struct task* current = tasks_manager.current;
    task_ready_list_append_one(current);
    task_schedule();
}

void task_sleep_until(int wait)
{
    struct task* current = tasks_manager.current;
    current->wait = wait;
    task_list_remove_one(&tasks_manager.ready_list, current);

    current->state = TASK_WAIT;
    task_list_add_one(&tasks_manager.wait_list, current);
    task_schedule();
}

void task_sleep(int wait)
{
    task_sleep_until(get_current_ticks() + wait);
}

void task_active(struct task* task)
{
    task_list_remove_one(&tasks_manager.wait_list, task);
    task->state = TASK_READY;
    task_ready_list_append_one(task);
}

void task_wake_up(int wait)
{
    struct task* task = tasks_manager.wait_list.next;
    struct task* next = 0;
    while (task)
    {
        next = task->next;
        if (task->wait <= wait)
        {
            task_list_remove_one(&tasks_manager.wait_list, task);
            task->state = TASK_READY;
            task_ready_list_append_one(task);
        }
        task = next;
    }
}

void task_free(struct task* task)
{
    free_paging(task->page_chunk);
    kfree(task_stack_bottom(task->t_stack, task->process->program_info.stack_size));
    kfree(task_stack_bottom(task->k_stack, 4 * PAGE_SIZE_4K));
    if (task->process->primary == task)
    {
        // free other related tasks
        struct task* next = task->th_next;
        while (next)
        {
            task_list_remove_one(&tasks_manager.terminated_list, next);
            free_paging(next->page_chunk);
            kfree(task_stack_bottom(next->t_stack, next->process->program_info.stack_size));
            kfree(task_stack_bottom(next->k_stack, 4 * PAGE_SIZE_4K));
            next = next->th_next;
        }
    }
    kfree(task);
}