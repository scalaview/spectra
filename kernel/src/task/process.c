#include "process.h"
#include "status.h"
#include "heap/kheap.h"
#include "task.h"
#include "config.h"
#include "kmemory.h"
#include "paging/paging.h"
#include "file.h"
#include "printk.h"
#include "assert.h"

static struct process* process_table[OS_MAX_PROCESSES];

struct process* get_current_process()
{
    assert(tasks_manager.current);
    return tasks_manager.current->process;
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

static int process_load_binary_program(const char* fullpath, struct process* process)
{
    int res = 0;
    FILE* fd;
    struct file_stat* stat;
    void* code_ptr;
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
    code_ptr = kzalloc(stat->filesize);
    if (!code_ptr)
    {
        res = -ENOMEM;
        goto out;
    }

    size_t size = fread(code_ptr, stat->filesize, 1, fd);
    if (size <= 0)
    {
        res = -EIO;
        goto out;
    }
    process->program_info.ptr = code_ptr;
    process->program_info.size = stat->filesize;
    process->program_info.stack_size = 4 * PAGE_SIZE_4K; //16K
    process->program_info.virtual_base_address = (void*)RANG_3_VMA;
    process->program_info.virtual_end_address = (void*)align_up(((uint64_t)process->program_info.virtual_base_address) + process->program_info.size);

out:
    fclose(fd);
    if (res < 0)
    {
        kfree(stat);
        kfree(code_ptr);
    }
    return res;
}

static int process_initialize_program(const char* fullpath, struct process* process)
{
    return process_load_binary_program(fullpath, process);
}

int process_initialize_task(struct process* process, struct task** out_task)
{
    int res = 0;
    struct task* task;
    task = create_task(process);
    if (task <= 0)
    {
        res = -ENOTASK;
        goto out;
    }
    if (!process->primary)
    {
        process->primary = task;
    }
    *out_task = task;
out:
    if (res < 0)
    {
        kfree(task);
    }
    return res;
}

int process_load(const int process_id, const char* fullpath, struct process** process_ptr)
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
    res = process_initialize_program(fullpath, process);
    if (res)
    {
        goto out;
    }
    struct task* task = 0;
    res = process_initialize_task(process, &task);
    if (res < 0)
    {
        goto out;
    }
    process->id = process_id;
    process->parent_id = 0;
    *process_ptr = process;
    process_table[process_id] = process;
out:
    if (res < 0)
    {
        kfree(process);
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

int process_launch(uint32_t pid)
{
    int res = 0;
    struct process* process = get_process(pid);
    if (!process)
    {
        res = -EISTAKEN;
        goto out;
    }
    task_list_remove_one(&tasks_manager.wait_list, process->primary);
    task_ready_list_append_one(process->primary);
out:
    return res;
}

void process_wake_up(int id)
{
    struct process* process = get_process(id);
    if (!process)
        return;
    struct task* task = process->primary;
    while (task)
    {
        if (task->state == TASK_WAIT)
        {
            task_active(task);
        }
        task = task->th_next;
    }
}

void process_free(struct process* process)
{

    struct process* child = process->children;
    struct process* prev = 0;

    while (child)
    {
        task_list_remove_one(&tasks_manager.terminated_list, child->primary);
        task_free(child->primary);
        prev = child;
        child = prev->children;
        kfree(prev);
    }
    task_list_remove_one(&tasks_manager.terminated_list, process->primary);
    task_free(process->primary);
    process_table[process->id] = 0;
    kfree(process);
}

void process_exit()
{
    struct task* cur_task = tasks_manager.current;
    struct process* master_process = get_process(cur_task->process->parent_id);
    if (!master_process)
        return;
    struct process* cur_process = cur_task->process;
    while (cur_process) {
        struct task* task = cur_process->primary;
        while (task)
        {
            if (task->state == TASK_READY)
            {
                task_list_remove_one(&tasks_manager.ready_list, task);
            }
            else if (task->state == TASK_WAIT)
            {
                task_list_remove_one(&tasks_manager.wait_list, task);
            }
            task->state = TASK_TERMINATE;
            task_list_add_one(&tasks_manager.terminated_list, task);
            task = task->th_next;
        }
        cur_process = cur_process->children;
    }
    process_wake_up(master_process->id); // 0 will be INIT process
    task_schedule();
}

int process_waitpid(int pid)
{
    if (is_list_empty(&tasks_manager.terminated_list))
    {
        return 0;
    }
    if (!pid)
        return 0;
    struct process* current_process = get_current_process();
    if (current_process->id == pid)
        return 0; // can't wait for self
    // wait for special pid
    int res = 0;
    struct process* process = process_table[pid];
    while (!res)
    {
        struct task* task = process->primary;
        res = 1;
        while (task)
        {
            if (task->state != TASK_TERMINATE)
            {
                res = 0;
                break;
            }
            task = task->th_next;
        }
        if (!res)
        {
            task_sleep(1);
        }
    }
    // wait for all task terminated
    process_free(process);
    return pid;
}

int process_wait(int pid)
{
    if (is_list_empty(&tasks_manager.terminated_list))
    {
        return 0;
    }
    if (pid)
    {
        // wait for special pid
        return process_waitpid(pid);
    }
    int32_t res = 0;
    while (1)
    {
        struct task* task = tasks_manager.terminated_list.next;
        struct process* current_process = get_current_process();
        while (task)
        {
            if (current_process->id == task->process->parent_id)
            {
                assert(task->state == TASK_TERMINATE);
                res = task->process->id;
                process_free(task->process);
                goto out;
            }
            task = task->next;
        }
        task_sleep(1);
    }

out:
    return res;
}

