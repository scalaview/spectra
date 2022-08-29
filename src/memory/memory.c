#include <stdint.h>

#include "memory.h"
#include "config.h"
#include "printk.h"

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


void get_memory_info()
{
    uint32_t block_size = *(uint32_t*)MEMORY_BLOCK_SIZE_ADDR;
    struct e820map* memory_map = (struct e820map*)MEMORY_INFO_ADDR;
    uint64_t total_mem = 0;
    for (int i = 0;i < block_size; i++)
    {
        uint64_t begin = memory_map[i].base_address;
        uint64_t end = begin + memory_map[i].length;
        if (memory_map[i].type == 1)
        {
            total_mem += memory_map[i].length;
        }
        printk("%x, %dKB [%x, %x], type: %d\n", memory_map[i].base_address, memory_map[i].length / 1024, begin, end - 1, (uint64_t)memory_map[i].type);
    }
    printk("Block size: %d, Total memory is %dByte, %dMB\n", block_size, total_mem, total_mem / 1024 / 1024);

}