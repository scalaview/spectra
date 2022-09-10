#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include <stddef.h>

typedef void (*INIT_FUNCTION)(void);
typedef void (*INIT_MEMMAP_FUNCTION)(void);
typedef void* (*ALLOC_FUNCTION)(size_t size);
typedef void (*FREE_FUNCTION)(void* ptr);

struct pmm_manager {
    const char* name;
    INIT_FUNCTION init;
    INIT_MEMMAP_FUNCTION init_kheap;
    ALLOC_FUNCTION alloc;
    FREE_FUNCTION free;
}__attribute__((packed));

extern const struct pmm_manager* pmm_manager;

void init_pmm();

#endif