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
    if (task->process->id == IDLE_PROCESS_ID) return;

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

static void __task_wait_list_move_to_ready(struct task* task)
{

    task_list_remove_one(&tasks_manager.wait_list, task);
    task->state = TASK_READY;
    task_ready_list_append_one(task);
}

uint8_t page_flags_by_ring(RING_LEV ring)
{
    uint8_t page_flag = PAGING_IS_WRITEABLE | PAGING_PRESENT | PAGING_ACCESS_FROM_ALL;
    if (ring == RING0)
    {
        page_flag = PAGING_IS_WRITEABLE | PAGING_PRESENT;
    }
    return page_flag;
}

static void task_initialize_stack(struct task* task, struct process* process)
{
    task->registers = (struct registers*)((uint64_t)task->k_stack - sizeof(struct registers));

    // adjust task_context_switch function
    task->k_context = (uint64_t)task->registers - 7 * 8;
    // set return address at restore_registers function
    *(uint64_t*)(task->k_context + 6 * 8) = (uint64_t)restore_registers;
    task->registers->cs = process->program_info.code_segement;
    task->registers->ss = process->program_info.data_segement;
    task->registers->rip = (uint64_t)process->program_info.virtual_base_address;
    task->registers->rflags = process->program_info.flags; // enable interrupt
    task->registers->rsp = (uint64_t)task->t_stack.stack_top;
    if (process->ring_lev == RING0)
    {
        task->registers->rsp = (uint64_t)task->k_stack;
    }
}

void* task_stack_bottom(void* stack, size_t size)
{
    if (!stack)
        return 0;
    return (void*)(((uint64_t)stack) - size);
}

static struct allocation* __task_malloc(struct task* task, struct process* process, size_t size)
{
    if (size <= 0) return 0;
    size_t aligned_size = align_up(size);
    void* kptr = kzalloc(aligned_size);
    if (!kptr) return 0;
    struct allocation* allocation = 0;
    int res = 0;
    uint64_t task_address = process->end_address;
    int64_t diff = ((uint64_t)task_address - (uint64_t)(process->program_info.virtual_end_address));
    if (diff < 0)
    {
        res = -EINVARG;
        goto out;
    }
    res = paging_initialize_pml4_table(&process->page_chunk, task_address, task_address + aligned_size, vir2phy(kptr), PAGE_SIZE_4K, page_flags_by_ring(process->ring_lev));
    if (res < 0) goto out;

    allocation = kzalloc(sizeof(struct allocation));
    if (!allocation)
    {
        res = -ENOMEM;
        goto out;
    }

    int64_t index = (diff / PAGE_SIZE_4K) % PROCESS_ALLOCATIONS;
    struct allocation_wrapper* allocation_wrapper = &process->allocations[index];
    if (!allocation_wrapper->next)
    {
        allocation_wrapper->next = allocation;
    }
    else
    {
        allocation_wrapper->tail->next = allocation;
    }
    allocation_wrapper->tail = allocation;
    allocation->kptr = kptr;
    allocation->tptr = (void*)task_address;
    allocation->size = aligned_size;
    process->end_address += aligned_size;

out:
    if (res < 0)
    {
        kfree(kptr);
        kfree(allocation);
        return 0;
    }

    return allocation;
}

struct allocation* task_malloc(struct task* task, size_t size)
{
    return __task_malloc(task, task->process, size);
}

int task_initialize(struct task* task, struct process* process)
{
    int res = 0;
    struct allocation* task_stack_allocation;
    void* kernel_stack;
    memset(task, 0, sizeof(struct task));
    task->process = process;
    kernel_stack = kzalloc(4 * PAGE_SIZE_4K);
    if (!kernel_stack)
    {
        res = -ENOMEM;
        goto out;
    }
    // map kernel page
    if (!process->page_chunk) process->page_chunk = kernel_paging_initialize();
    if (!process->page_chunk)
    {
        res = -ENOMEM;
        goto out;
    }

    // initialize task memory space.
    res = paging_initialize_pml4_table(&process->page_chunk, (uint64_t)process->program_info.virtual_base_address, (uint64_t)process->program_info.virtual_end_address, vir2phy(process->program_info.ptr), PAGE_SIZE_4K, page_flags_by_ring(process->ring_lev));
    if (res < 0)
    {
        goto out;
    }

    if (process->ring_lev == RING3)
    {
        task_stack_allocation = process->mmu.malloc(task, process->program_info.stack_size);
        if (!task_stack_allocation)
        {
            res = -EMALLOC;
            goto out;
        }
        task->t_stack.heap_address = task_stack_allocation->kptr;
        task->t_stack.stack_bottom = task_stack_allocation->tptr;
        task->t_stack.stack_top = (void*)(((uint64_t)task_stack_allocation->tptr) + process->program_info.stack_size);
    }

    task->k_stack = (void*)(((uint64_t)kernel_stack) + 4 * PAGE_SIZE_4K);

    task_initialize_stack(task, process);
    task->state = TASK_WAIT;
    if (process->primary)
    {
        struct task* tptr = process->primary;
        //TODO support thread task
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
        if (!task_stack_allocation) process->mmu.free(process, task_stack_allocation->tptr);
        kfree(process->page_chunk);
        task->process = 0;
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
    switch_vm(task->process->page_chunk);
    // set_user_registers();
    task_start(task->registers);
}

void task_switch(struct task* next)
{
    struct task* current = task_list_current();
    task_list_set_current(next);
    set_tss_rsp0((uint64_t)next->k_stack);
    switch_vm(next->process->page_chunk);
    // set_user_registers();
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
        struct process* idle_process = get_process(IDLE_PROCESS_ID);
        assert(idle_process);
        struct task* task = idle_process->primary;
        task_switch(task);
        return;
    }
    struct task* task_head = task_remove_ready_list_head();
    task_head->state = TASK_RUNNING;
    task_switch(task_head);
}

void yield()
{
    struct task* current = task_list_current();
    if (current->process->id == IDLE_PROCESS_ID && is_list_empty(&tasks_manager.ready_list)) return;
    task_ready_list_append_one(current);
    task_schedule();
}

void task_sleep_until(int wait)
{
    struct task* current = task_list_current();
    current->wait = wait;
    task_list_remove_one(&tasks_manager.ready_list, current);

    current->state = TASK_WAIT;
    task_list_add_one(&tasks_manager.wait_list, current);
    task_schedule();
}

void task_sleep(int wait)
{
    if (wait > 0)
    {
        task_sleep_until(get_current_ticks() + wait);
    }
    else
    {
        task_sleep_until(wait);
    }
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
        if (wait > 0 && task->wait > 0 && task->wait <= wait)
        {
            __task_wait_list_move_to_ready(task);
        }
        else if (task->wait == wait)
        {
            __task_wait_list_move_to_ready(task);
        }
        task = next;
    }
}

void task_free(struct task* task)
{
    task->process->mmu.free(task->process, task->t_stack.stack_bottom);
    kfree(task_stack_bottom(task->k_stack, 4 * PAGE_SIZE_4K));
    if (task->process->primary == task)
    {
        // free other related tasks
        struct task* next = task->th_next;
        //TODO support thread task
        while (next)
        {
            task_list_remove_one(&tasks_manager.terminated_list, next);
            task->process->mmu.free(task->process, next->t_stack.stack_bottom);
            kfree(task_stack_bottom(next->k_stack, 4 * PAGE_SIZE_4K));
            next = next->th_next;
        }
    }
    kfree(task);
}

int task_clone(struct task* src, struct task* dest)
{
    if (!src || !dest) return -EINVARG;
    if (src->t_stack.heap_address)
    {
        memcpy(dest->t_stack.heap_address, src->t_stack.heap_address, src->process->program_info.stack_size);
    }
    memcpy(dest->registers, src->registers, sizeof(struct registers));
    return 0;
}

void task_apply_arguments_to_registers(struct task* task)
{
    task->registers->rdi = (uint64_t)task->process->arguments.argc;
    task->registers->rsi = (uint64_t)task->process->arguments.argv;
}