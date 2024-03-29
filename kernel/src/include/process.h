#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <stddef.h>

#include "config.h"
#include "task/mmu.h"
#include "command_argument.h"
#include "drivers/keyboard/keyboard.h"

#define OS_MAX_PROCESSES        16
#define IDLE_PROCESS_ID         0
#define PROGRAME_MAX_FILEPATH   1024

typedef enum
{
    RING0,
    RING1,
    RING2,
    RING3,
} RING_LEV;

struct program_info
{
    void* ptr;
    char filename[PROGRAME_MAX_FILEPATH];
    size_t size;
    void* virtual_base_address;
    void* virtual_end_address;
    size_t stack_size;
    uint64_t code_segement;
    uint64_t data_segement;
    uint64_t flags;
};

struct allocation
{
    struct allocation* next;
    void* tptr;
    void* kptr;
    size_t size;
};

struct allocation_wrapper
{
    struct allocation* next;
    struct allocation* tail;
};

struct process_arguments
{
    int argc;
    char** argv;
};

struct process
{
    uint16_t id;
    uint16_t parent_id;
    RING_LEV ring_lev;
    struct pml4_table* page_chunk;
    struct task* primary;
    struct program_info program_info;
    struct process* children;
    struct allocation_wrapper allocations[PROCESS_ALLOCATIONS];
    uint64_t end_address;
    struct process_mmu mmu;
    struct process_arguments arguments;
};

int create_kernel_process(const char* fullpath, struct process** process);
int create_user_process(const char* fullpath, struct process** process);
int process_launch(uint32_t pid);
int process_initialize_task(struct process* process, struct task** out_task);
void process_exit();
int process_wait(int pid);
int process_fork();
int process_execve(const char* pathname, const char* argv[], const char* envp[], RING_LEV ring_lev);
struct allocation* process_alloc(size_t size);
void* process_malloc(size_t size);
void* process_internal_alloc(size_t size);
void process_malloc_free(void* task_address);
int process_inject_arguments(struct process* process, struct command_argument* root_argument);
struct process* get_process(int process_id);
void init_idle_process();
struct allocation* process_fetch_allocation(struct process* process, void* task_address);

#endif