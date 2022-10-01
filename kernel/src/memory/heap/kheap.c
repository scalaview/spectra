#include "heap/kheap.h"
#include "pmm.h"
#include "kmemory.h"

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

void* kzalloc(size_t size)
{
    void* ptr = kmalloc(size);

    if (!ptr) {
        return 0;
    }
    memset(ptr, 0x00, size);
    return ptr;
}