#include "task.h"
#include "kmemory.h"
#include "heap/kheap.h"
#include "status.h"
#include "paging/paging.h"
#include "process.h"
#include "assert.h"
#include "tss.h"

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
    tasks_manager.current = task;
}

bool is_list_empty(struct task_wrapper* list)
{
    return (list->next == NULL);
}

void task_ready_list_append_one(struct task* task)
{
    task->state = READY;
    task_list_add_one(&tasks_manager.ready_list, task);
}

void task_list_add_one(struct task_wrapper* list, struct task* task)
{
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
    int res = 0;
    struct task* current = list->next;
    if (current == task)
    {
        list->next = current->next;
        if (list->tail == current)
        {
            list->tail = current->prev;
        }
        else
        {
            current->next->prev = NULL;
        }
        res = SUCCESS;
        goto out;
    }

    if (list->tail == task)
    {
        list->tail = task->prev;
        res = SUCCESS;
        goto out;
    }

    while (current)
    {
        if (current->next == task)
        {
            current->next = task->next;
            task->prev = current;
            res = SUCCESS;
            break;
        }
        current = current->next;
    }

out:
    if (res)
    {
        task->prev = NULL;
        task->next = NULL;
    }
}

static void task_save_state(struct task* task, struct interrupt_frame* frame)
{
    task->registers.rax = frame->rax;
    task->registers.rbx = frame->rbx;
    task->registers.rcx = frame->rcx;
    task->registers.rdx = frame->rdx;
    task->registers.rsi = frame->rsi;
    task->registers.rdi = frame->rdi;
    task->registers.rbp = frame->rbp;
    task->registers.r8 = frame->r8;
    task->registers.r9 = frame->r9;
    task->registers.r10 = frame->r10;
    task->registers.r11 = frame->r11;
    task->registers.r12 = frame->r12;
    task->registers.r13 = frame->r13;
    task->registers.r14 = frame->r14;
    task->registers.r15 = frame->r15;

    task->registers.rip = frame->rip;
    task->registers.cs = frame->cs;
    task->registers.rflags = frame->rflags;
    task->registers.rsp = frame->rsp;
    task->registers.ss = frame->ss;
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

    task->kstack = (void*)(((char*)kernel_stack) + 4 * PAGE_SIZE_4K);
    task->tstack = (void*)(((char*)task_stack) + process->program_info.stack_size);
    task->registers.cs = USER_CODE_SEGMENT | 3;
    task->registers.ss = USER_DATA_SEGMENT | 3;
    task->registers.rsp = RANG_3_STACK_PTR;
    task->registers.rip = RANG_3_VMA;
    task->registers.rflags = 0x202; // enable interrupt
    task->state = WAIT;
    task->process = process;
    if (process->primary)
    {
        struct task* tptr = process->primary;
        if (tptr->thead)
        {
            task->thead = tptr->thead;
        }
        tptr->thead = task; // link treads
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
    set_tss_rsp0((uint64_t)task->kstack);
    switch_vm(task->page_chunk);
    set_user_registers();
    task_switch(&task->registers);
}

void task_run_schedule()
{
    if (is_list_empty(&tasks_manager.ready_list))
    {
        return;
    }
    struct task* task_head = tasks_manager.ready_list.next;
    task_list_remove_one(&tasks_manager.ready_list, task_head);
    task_head->state = RUNNING;
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
    task_run_schedule();
}



void task_run_next()
{
    if (is_list_empty(&tasks_manager.ready_list))
    {
        return;
    }
    yield();
}