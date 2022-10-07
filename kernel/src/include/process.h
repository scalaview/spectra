#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <stddef.h>

#define OS_MAX_PROCESSES        16
#define INIT_PROCESS_ID         0

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
    size_t size;
    void* virtual_base_address;
    void* virtual_end_address;
    size_t stack_size;
    uint64_t code_segement;
    uint64_t data_segement;
    uint64_t flags;
};

struct process
{
    uint16_t id;
    uint16_t parent_id;
    RING_LEV ring_lev;
    struct task* primary;
    struct program_info program_info;
    struct process* children;
};

int create_kernel_process(const char* fullpath, struct process** process);
int create_user_process(const char* fullpath, struct process** process);
int process_launch(uint32_t pid);
int process_initialize_task(struct process* process, struct task** out_task);
void process_exit();
int process_wait(int pid);
int process_clone(struct process* src, struct process** dest);
int process_fork();

#endif