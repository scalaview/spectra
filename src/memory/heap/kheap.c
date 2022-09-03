#include "heap/kheap.h"
#include "pmm.h"


void kernel_heap_initialize()
{
    init_pmm();
    pmm_manager->init_kheap();
}

void* kmalloc(size_t size)
{
    return pmm_manager->alloc(size);
}

void kfree(void* ptr)
{
    pmm_manager->free(ptr);
}
