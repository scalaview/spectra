#ifndef KHEAP_H
#define KHEAP_H

#include <stddef.h>

void kernel_heap_initialize();
void* kmalloc(size_t size);
void* kzalloc(size_t size);
void kfree(void* ptr);

#endif