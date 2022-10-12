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
#include "task/none.h"
#include "string.h"

static struct process* process_table[OS_MAX_PROCESSES];

struct process* get_current_process()
{
    assert(task_list_current());
    return task_list_current()->process;
}

int get_unused_process_index()
{
    for (int i = INIT_PROCESS_ID; i < OS_MAX_PROCESSES; i++)
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

out:
    fclose(fd);
    kfree(stat);
    if (res < 0)
    {
        kfree(code_ptr);
    }
    return res;
}

static int process_initialize_binary_program(const char* fullpath, struct process* process, RING_LEV ring_level)
{
    int res = 0;
    if (fullpath)
    {
        res = process_load_binary_program(fullpath, process);
        if (res < 0)
        {
            goto out;
        }
    }
    struct program_info* program_info = &process->program_info;
    if (ring_level == RING0) // kernel land
    {
        program_info->virtual_base_address = (void*)RING_0_VMA;
        program_info->virtual_end_address = (void*)align_up(((uint64_t)program_info->virtual_base_address) + program_info->size);
        program_info->code_segement = KERNEL_CODE_SEGMENT;
        program_info->data_segement = KERNEL_DATA_SEGMENT;
    }
    else if (ring_level == RING3) // userland
    {
        program_info->virtual_base_address = (void*)RING_3_VMA;
        program_info->virtual_end_address = (void*)align_up(((uint64_t)program_info->virtual_base_address) + program_info->size);
        program_info->code_segement = USER_CODE_SEGMENT | 3;
        program_info->data_segement = USER_DATA_SEGMENT | 3;
    }
    program_info->stack_size = 4 * PAGE_SIZE_4K; //16K
    process->end_address = (uint64_t)program_info->virtual_end_address;
    program_info->flags = 0x202;
    process->ring_lev = ring_level;
out:
    return res;
}

static int process_initialize_program(const char* fullpath, struct process* process, RING_LEV ring_level)
{
    return process_initialize_binary_program(fullpath, process, ring_level);
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

int process_load(const int process_id, const char* fullpath, struct process* process, RING_LEV ring_level)
{
    int res = 0;
    if (get_process(process_id) != 0)
    {
        res = -EISTAKEN;
        goto out;
    }

    res = process_initialize_program(fullpath, process, ring_level);
    if (res < 0)
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
    process_table[process_id] = process;
out:
    return res;
}

static int __setup_process_mmu(struct process* process, int index)
{
    memcpy(process->mmu.name, "none", 4);
    process->mmu.malloc = &none_malloc;
    process->mmu.free = &none_free;
    return 0;
}

int process_initialize(const char* fullpath, struct process** process, RING_LEV ring_level)
{
    int res = 0;
    struct process* new_process;
    int process_id = get_unused_process_index();
    if (process_id < 0)
    {
        res = process_id;
        goto out;
    }

    new_process = kzalloc(sizeof(struct process));

    if (!new_process)
    {
        res = -ENOMEM;
        goto out;
    }
    res = __setup_process_mmu(new_process, 0);
    if (res < 0) goto out;

    res = process_load(process_id, fullpath, new_process, ring_level);
    if (!res)
    {
        struct task* current = task_list_current();
        if (current)
        {
            // create by execve system call
            new_process->parent_id = current->process->id;
        }
        *process = new_process;
    }
out:
    if (res < 0)
    {
        kfree(new_process);
    }
    return res;
}

int create_kernel_process(const char* fullpath, struct process** process)
{
    return process_initialize(fullpath, process, RING0);
}

int create_user_process(const char* fullpath, struct process** process)
{
    return process_initialize(fullpath, process, RING3);
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
    //TODO support thread task
    while (task)
    {
        if (task->state == TASK_WAIT)
        {
            task_active(task);
        }
        task = task->th_next;
    }
}

static void __allocations_free(struct process* process)
{
    if (process->end_address <= (uint64_t)process->program_info.virtual_end_address) return;

    struct allocation_wrapper* allocation_wrapper = process->allocations;
    for (int i = 0; i < PROCESS_ALLOCATIONS; i++)
    {
        struct allocation* next = allocation_wrapper[i].next;
        if (!next) continue;
        struct allocation* prev = next;
        while (next)
        {
            prev = next;
            next = next->next;
            kfree(prev);
        }
        kfree(next);
    }
}

static void __process_free(struct process* process)
{

    struct process* child = process->children;
    struct process* prev = 0;

    while (child)
    {
        task_list_remove_one(&tasks_manager.terminated_list, child->primary);
        task_free(child->primary);
        prev = child;
        child = prev->children;
        kfree(prev->program_info.ptr);
        __allocations_free(prev);
        kfree(prev);
    }
    task_list_remove_one(&tasks_manager.terminated_list, process->primary);
    task_free(process->primary);
    process_table[process->id] = 0;
    kfree(process->program_info.ptr);
    __allocations_free(process);
    kfree(process->page_chunk);
    kfree(process);
}

void process_exit()
{
    struct task* cur_task = task_list_current();
    struct process* master_process = get_process(cur_task->process->parent_id);
    if (!master_process)
        return;
    struct process* cur_process = cur_task->process;
    while (cur_process) {
        struct task* task = cur_process->primary;
        //TODO support thread task
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
        //TODO support thread task
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
    __process_free(process);
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
                __process_free(task->process);
                goto out;
            }
            task = task->next;
        }
        task_sleep(1);
    }
out:
    return res;
}

static int __clone_process_allocations(struct process* dest, struct process* src)
{
    int res = 0;
    struct allocation_wrapper* src_wrapper = src->allocations;
    struct allocation_wrapper* dest_wrapper = dest->allocations;

    struct allocation* dest_prev = 0;
    struct allocation* new_alloc = 0;

    for (int i = 0; i < PROCESS_ALLOCATIONS; i++)
    {
        struct allocation* src_prev = src_wrapper[i].next;
        struct allocation* src_next = src_prev;

        dest_prev = 0;
        new_alloc = 0;

        while (src_next)
        {
            new_alloc = kzalloc(sizeof(struct allocation));
            if (!new_alloc)
            {
                res = -ENOMEM;
                goto out;
            }
            new_alloc->kptr = kzalloc(src_next->size);
            if (!new_alloc->kptr)
            {
                res = -ENOMEM;
                goto out;
            }
            res = paging_initialize_pml4_table(&dest->page_chunk, (uint64_t)src_next->tptr, ((uint64_t)src_next->tptr) + src_next->size, vir2phy(new_alloc->kptr), PAGE_SIZE_4K, page_flags_by_ring(dest->ring_lev));
            if (res < 0) goto out;

            if (src_prev == src_next) // is the first one
            {
                dest_wrapper[i].next = new_alloc;
            }
            src_prev = src_next;
            src_next = src_prev->next;
            memcpy(new_alloc->kptr, src_prev->kptr, src_prev->size);
            new_alloc->tptr = src_prev->tptr;
            new_alloc->size = src_prev->size;
            if (dest_prev) dest_prev->next = new_alloc;
            dest_prev = new_alloc;
        }
        dest_wrapper[i].tail = dest_prev;
    }
out:
    if (res < 0)
    {
        kfree(new_alloc);
        __allocations_free(dest);
    }
    return res;
}

static int __clone_program(struct program_info* dest, struct program_info* src)
{
    int res = 0;
    dest->ptr = kzalloc(src->size);
    if (!dest->ptr)
    {
        res = -ENOMEM;
        goto out;
    }
    memcpy(dest->ptr, src->ptr, src->size);
    dest->size = src->size;
    dest->virtual_base_address = src->virtual_base_address;
    dest->virtual_end_address = src->virtual_end_address;
    dest->stack_size = src->stack_size;
    dest->code_segement = src->code_segement;
    dest->data_segement = src->code_segement;
    dest->flags = src->flags;
out:
    return res;
}

static void process_clone_mmu(struct process* src, struct process* dest)
{
    memcpy(dest->mmu.name, src->mmu.name, strlen(src->mmu.name));
    dest->mmu.malloc = src->mmu.malloc;
    dest->mmu.free = src->mmu.free;
}

static int process_clone(struct process* src, struct process** dest)
{
    int res = 0;
    struct process* process = kzalloc(sizeof(struct process));
    if (!process)
    {
        res = -ENOMEM;
        goto out;
    }

    process->parent_id = src->id;
    process_clone_mmu(src, process);
    res = __clone_program(&process->program_info, &src->program_info);
    if (res < 0)  goto out;
    res = process_load(get_unused_process_index(), NULL, process, src->ring_lev);
    if (res < 0)
    {
        goto out;
    }
    res = __clone_process_allocations(process, src);
    if (res < 0) goto out;
    /**
     * setup the task IP to correct position
     * and copy all the source task stack to the fork one
     */
     // TODO clone all threads
    res = task_clone(src->primary, process->primary); // only clone the primary one
    if (res < 0) goto out;
    *dest = process;

out:
    if (res < 0)
    {
        kfree(process);
    }
    return res;
}


int process_fork()
{
    struct process* current = get_current_process();
    struct process* new_process = 0;
    int res = process_clone(current, &new_process);
    if (res < 0)
    {
        goto out;
    }
    res = process_launch(new_process->id);
out:
    if (res < 0)
    {
        kfree(new_process);
        return 0;
    }

    return new_process->id;
}

int process_execve(const char* pathname, const char* argv[], const char* envp[], RING_LEV ring_lev)
{
    struct command_argument* root_argv = parser_argument_array((const void**)argv);
    struct command_argument* root_envp = parser_argument_array((const void**)envp);
    if (root_argv || root_envp);

    struct process* current = get_current_process();
    int res = 0;
    if (ring_lev < current->ring_lev)
    {
        res = -EINVARG;
        goto out;
    }
    struct process* process = 0;
    res = process_initialize(pathname, &process, ring_lev);
    if (res < 0)
    {
        goto out;
    }
    if (root_argv) process_inject_arguments(process, root_argv);
    if (root_envp) process_inject_arguments(process, root_envp);

    process_launch(process->id);
out:
    command_argument_free(root_argv);
    command_argument_free(root_envp);
    return res;
}

void* process_malloc(size_t size)
{
    struct process* process = get_current_process();
    if (!process) return 0;

    struct allocation* allocation = process->mmu.malloc(task_list_current(), size);
    if (allocation) return allocation->tptr;
    return 0;
}

void __process_allocation_free(struct process* process, void* task_address)
{
    int64_t diff = ((uint64_t)task_address - (uint64_t)(process->program_info.virtual_end_address));
    if (diff < 0) return;
    int64_t index = (diff / PAGE_SIZE_4K) % PROCESS_ALLOCATIONS;
    struct allocation_wrapper* allocation_wrapper = &process->allocations[index];

    struct allocation* prev = allocation_wrapper->next;
    struct allocation* next = prev;

    while (next)
    {
        if (next->tptr == task_address)
        {
            if (allocation_wrapper->next == next)
            {
                allocation_wrapper->next = next->next;
            }
            if (allocation_wrapper->tail == next)
            {
                allocation_wrapper->tail = prev == next ? NULL : prev;
            }
            prev->next = next->next;
            break;
        }
        prev = next;
        next = next->next;
    }

    if (next)
    {
        kfree(next->kptr);
        kfree(next);
    }
    return;
}

void process_malloc_free(void* task_address)
{
    struct process* process = get_current_process();
    if (!process) return;

    process->mmu.free(process, task_address);
}

static int process_count_command_arguments(struct command_argument* root_argument)
{
    struct command_argument* current = root_argument;
    int i = 0;
    while (current)
    {
        i++;
        current = current->next;
    }
    return i;
}

int process_inject_arguments(struct process* process, struct command_argument* root_argument)
{
    int res = 0;
    struct command_argument* current = root_argument;
    int i = 0;
    char** argv = 0;
    int argc = process_count_command_arguments(root_argument);
    if (argc == 0)
    {
        res = -EIO;
        goto out;
    }

    struct allocation* argv_allocation = process->mmu.malloc(process->primary, sizeof(const char*) * argc);
    argv = argv_allocation->kptr;
    if (!argv)
    {
        res = -ENOMEM;
        goto out;
    }

    while (current)
    {
        struct allocation* arg_allocation = process->mmu.malloc(process->primary, sizeof(current->argument));
        char* argument_str = (char*)arg_allocation->kptr;
        if (!argument_str)
        {
            res = -ENOMEM;
            goto out;
        }
        strncpy(argument_str, current->argument, sizeof(current->argument));
        argv[i] = arg_allocation->tptr;
        current = current->next;
        i++;
    }

    process->arguments.argc = argc;
    process->arguments.argv = (char**)argv_allocation->tptr;

    task_apply_arguments_to_registers(process->primary);

out:
    if (res < 0)
    {
        if (argv)
        {
            for (int i = 0; i < argc;i++)
            {
                if (!argv[i]) break;
                process->mmu.free(process, argv[i]);
            }
        }
    }
    return res;
}