#ifndef PROCESS_MMU_NONE_H
#define PROCESS_MMU_NONE_H

#include "process.h"

struct allocation* none_malloc(struct task* task, size_t size);
void none_free(struct process* process, void* ptr);

#endif