#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <stddef.h>

#define OS_MAX_PROCESSES        16

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
    struct task* primary;
    struct program_info program_info;
};

int process_initialize(struct process** process);
int process_launch(uint32_t pid);

#endif