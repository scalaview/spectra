#include <stdint.h>

#include "kmemory.h"
#include "config.h"
#include "printk.h"

extern void* kernel_end;
struct mem_map mem_map;

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

void init_memory_map()
{
    mem_map.block_size = *(uint32_t*)MEMORY_BLOCK_SIZE_ADDR;
    mem_map.map = (struct e820map*)MEMORY_INFO_ADDR;
}

struct mem_map* get_memory_map()
{
    return &mem_map;
}

void get_memory_info()
{
    struct mem_map* mem_map = get_memory_map();
    uint32_t block_size = mem_map->block_size;
    uint64_t total_mem = 0;
    for (struct e820map* memory_map = mem_map->map; memory_map < mem_map->map + block_size; memory_map++)
    {
        uint64_t begin = memory_map->base_address;
        uint64_t end = begin + memory_map->length;
        if (memory_map->type == 1)
        {
            total_mem += memory_map->length;
        }
        printk("%x, %dKB [%x, %x], type: %d\n", memory_map->base_address, memory_map->length / 1024, begin, end - 1, (uint64_t)memory_map->type);
    }
    printk("Block size: %d, Total memory is %dByte, %dMB\n", block_size, total_mem, total_mem / 1024 / 1024);
    printk("Kernel end: %x\n", vir2phy((uint64_t)&kernel_end));

}