#include "process.h"
#include "status.h"
#include "heap/kheap.h"
#include "task.h"
#include "config.h"
#include "tss.h"
#include "kmemory.h"
#include "paging/paging.h"
#include "file.h"
#include "printk.h"

static struct process* process_table[OS_MAX_PROCESSES];

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

static int process_load_binary(const char* fullpath, struct process* process)
{
    int res = 0;
    FILE* fd;
    struct file_stat* stat;
    fd = fopen(fullpath, "r");
    if (!fd->fdi)
    {
        res = -EIO;
        goto out;
    }
    stat = kzalloc(sizeof(struct file_stat));
    if (!stat)
    {
        res = -ENOMEM;
        goto out;
    }
    res = fstat(fd->fdi, stat);
    if (res)
    {
        goto out;
    }
    struct task* task = process->primary;
    if (!task)
    {
        res = -EINVARG;
        goto out;
    }

    size_t size = fread(task->entry, stat->filesize, 1, fd);
    if (size <= 0)
    {
        res = -EIO;
        goto out;
    }

out:
    fclose(fd);
    if (stat)
    {
        kfree(stat);
    }
    return res;
}

int process_load_data(const char* fullpath, struct process* process)
{
    return process_load_binary(fullpath, process);
}

int process_read_binary_program_state(const char* fullpath, struct process* process)
{
    int res = 0;
    FILE* fd;
    struct file_stat* stat;
    fd = fopen(fullpath, "r");
    if (!fd->fdi)
    {
        res = -EIO;
        goto out;
    }
    stat = kzalloc(sizeof(struct file_stat));
    if (!stat)
    {
        res = -ENOMEM;
        goto out;
    }
    res = fstat(fd->fdi, stat);
    if (res)
    {
        goto out;
    }
    process->program_info.size = stat->filesize;
    process->program_info.stack_size = 4 * PAGE_SIZE_4K; //16K
    process->program_info.virtual_base_address = (void*)RANG_3_VMA;
    process->program_info.virtual_end_address = (void*)align_up(((uint64_t)process->program_info.virtual_base_address) + process->program_info.size);
out:
    fclose(fd);
    if (stat)
    {
        kfree(stat);
    }
    return res;
}

int process_initialize_program(const char* fullpath, struct process* process)
{
    return process_read_binary_program_state(fullpath, process);
}

int process_load(const int process_id, const char* fullpath, struct process** process_ptr)
{
    int res = 0;
    struct process* process;
    struct task* task;
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
    res = process_initialize_program(fullpath, process);
    if (res)
    {
        goto out;
    }

    task = create_task(process);
    if (task <= 0)
    {
        res = -ENOTASK;
        goto out;
    }

    process->primary = task;
    res = process_load_data(fullpath, process);
    if (res < 0)
    {
        goto out;
    }
    *process_ptr = process;
    process_table[process_id] = process;
out:
    if (res < 0)
    {
        kfree(task);
    }
    return res;
}

int process_initialize(const char* fullpath, struct process** process)
{
    int res = 0;
    int process_id = get_unused_process_index();
    if (process_id < 0)
    {
        res = process_id;
        goto out;
    }

    res = process_load(process_id, fullpath, process);
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
    set_tss_rsp0((uint64_t)process->primary->kstack);
    switch_vm(process->primary->page_chunk);
    set_user_registers();
    task_launch(process->primary);
out:
    return res;
}

