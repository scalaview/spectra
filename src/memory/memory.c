#include <stdint.h>

#include "memory.h"

void* memset(void* ptr, int c, size_t size)
{
    char* ch_ptr = (char*)ptr;
    for (int i = 0;i < size; i++)
    {
        ch_ptr[i] = (char)c;
    }
    return ptr;
}

void* memcpy(void* dest, void* src, int len)
{
    char* d = dest;
    char* s = src;

    while (len--)
    {
        *d++ = *s++;
    }
    return dest;
}

// 00h    QWORD   base address
// 08h    QWORD   length in bytes
// 10h    DWORD   type of address range (see #00581)
struct memory_map {
    uint64_t base_address;
    uint64_t lenght;
    uint32_t type;
} __attribute__((packed));

