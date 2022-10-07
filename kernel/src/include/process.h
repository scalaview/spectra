#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <stddef.h>

#define OS_MAX_PROCESSES        16
#define INIT_PROCESS_ID         0


struct program_info
{
    void* ptr;
    size_t size;
    void* virtual_base_address;
    void* virtual_end_address;
    size_t stack_size;
};

struct process
{
    uint16_t id;
    uint16_t parent_id;
    struct task* primary;
    struct program_info program_info;
    struct process* children;
};

int process_initialize(const char* fullpath, struct process** process);
int process_launch(uint32_t pid);
int process_initialize_task(struct process* process, struct task** out_task);
void process_exit();
int process_wait(int pid);

#endif