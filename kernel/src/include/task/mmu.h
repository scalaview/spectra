#ifndef PROCESS_MMU_H
#define PROCESS_MMU_H

#include <stddef.h>
struct task;
struct allocation;
struct process;

typedef struct allocation* (*MALLOC_FUNCTION)(struct task* task, size_t size);
typedef void (*FREE_FUNCTION)(struct process* process, void* ptr);

struct process_mmu
{
    char name[64];
    MALLOC_FUNCTION malloc;
    FREE_FUNCTION free;
};

#endif