#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdint.h>

#include "config.h"

/**
* 00h    QWORD   base address
* 08h    QWORD   length in bytes
* 10h    DWORD   type of address range (see #00581)
*/
struct e820map {
    uint64_t base_address;
    uint64_t length;
    uint32_t type;
} __attribute__((packed));

#define phy2vir(p) ((uint64_t)(p) + KERNEL_VM_BASE)

void* memset(void* ptr, int c, size_t size);
void* memcpy(void* dest, void* src, int len);
void get_memory_info();

#endif