#include "task.h"
#include "kmemory.h"
#include "heap/kheap.h"
#include "status.h"
#include "paging/paging.h"
#include "process.h"
#include "assert.h"
#include "tss.h"

struct tasks_manager tasks_manager = {
    .head = NULL,
    .current = NULL,
    .tail = NULL
};

struct task* task_list_current()
{
    struct task_wrapper* task_wrapper = tasks_manager.current;
    if (!task_wrapper)
        return 0;
    return task_wrapper->task;
};

int task_list_set_current(struct task* task)
{
    struct task_wrapper* task_wrapper = kzalloc(sizeof(struct task_wrapper));
    if (!task_wrapper)
    {
        return -ENOMEM;
    }
    task_wrapper->task = task;
    tasks_manager.current = task_wrapper;
    return 0;
}

int task_list_add_one(struct task* task)
{
    struct task_wrapper* task_wrapper = kzalloc(sizeof(struct task_wrapper));
    if (!task_wrapper)
    {
        return -ENOMEM;
    }
    task_wrapper->task = task;
    if (!tasks_manager.head)
    {
        tasks_manager.head = task_wrapper;
    }
    if (tasks_manager.tail)
    {
        task_wrapper->prev = tasks_manager.tail;
    }
    tasks_manager.tail = task_wrapper;
    return 0;
};



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
    task->process = process;
    if (process->primary)
    {
        struct task* tptr = process->primary;
        while (tptr->next)
        {
            tptr = tptr->next;
        }
        tptr->next = task;
        task->prev = tptr;
    }

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

int task_launch(struct task* task)
{
    int res = task_list_set_current(task);
    if (res < 0)
        return res;
    set_tss_rsp0((uint64_t)task->kstack);
    switch_vm(task->page_chunk);
    set_user_registers();
    task_switch(&task->registers);
    return 0;
}