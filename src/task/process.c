#include "process.h"
#include "status.h"
#include "heap/kheap.h"
#include "task.h"
#include "config.h"
#include "tss.h"
#include "kmemory.h"
#include "paging/paging.h"

static struct process* process_table[OS_MAX_PROCESSES];

void main()
{
    char* p = (char*)0xffff800000200020;
    *p = 1;
}

int get_unused_process_index()
{
    for (int i = 0; i < OS_MAX_PROCESSES; i++)
    {
        if (process_table[i] == 0)
        {
            return i;
        }
    }
    return -EISTAKEN;
}

struct process* get_process(int process_id)
{
    if (process_id < 0 || process_id >= OS_MAX_PROCESSES)
    {
        return 0;
    }
    return process_table[process_id];
}

void process_map_program(struct process* process)
{
    memcpy(process->primary->tptr, process->program_info.ptr, process->program_info.size);
}

int process_load(const int process_id, struct process** process_ptr)
{
    int res = 0;
    struct process* process;

    if (get_process(process_id) != 0)
    {
        res = -EISTAKEN;
        goto out;
    }
    process = kzalloc(sizeof(struct process));

    if (!process)
    {
        res = -ENOMEM;
        goto out;
    }
    process->id = process_id;
    process->program_info.size = 2 * PAGE_SIZE_2M;
    process->program_info.stack_size = 4 * PAGE_SIZE_4K;
    process->program_info.virtual_base_address = (void*)RANG_3_VMA;
    process->program_info.virtual_end_address = process->program_info.virtual_base_address + process->program_info.size;
    process->program_info.ptr = &main;
    struct task* task = create_task(process);
    if (task <= 0)
    {
        res = -ENOTASK;
        goto out;
    }

    process->primary = task;
    process_map_program(process);
    *process_ptr = process;
    process_table[process_id] = process;
out:
    return res;
}

int process_initialize(struct process** process)
{
    int res = 0;
    int process_id = get_unused_process_index();
    if (process_id < 0)
    {
        res = process_id;
        goto out;
    }

    res = process_load(process_id, process);
out:
    return res;
}

void switch_vm(struct pml4_table* pml4_table)
{
    setup_paging_directory(vir2phy(pml4_table->entries));
}

int process_launch(uint32_t pid)
{
    int res = 0;
    struct process* process = get_process(pid);
    if (!process)
    {
        res = -EISTAKEN;
        goto out;
    }
    set_tss_rsp0((uint64_t)process->primary->tptr);
    switch_vm(process->primary->page_chunk);
    set_user_registers();
    task_launch(process->primary);
out:
    return res;
}

