#include <stdint.h>

#include "kmemory.h"
#include "config.h"
#include "printk.h"
#include "assert.h"

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

void init_memory_map(struct multiboot_tag_mmap* tag)
{
    extern void* mbimemory_info;
    struct e820map* e_map = (struct e820map*)(mbimemory_info);
    uint32_t size = 0;
    mem_map.map = e_map;
    multiboot_memory_map_t* mmap;
    for (mmap = ((struct multiboot_tag_mmap*)tag)->entries;
        (multiboot_uint8_t*)mmap
        < (multiboot_uint8_t*)tag + tag->size;
        mmap = (multiboot_memory_map_t*)
        ((unsigned long)mmap
            + ((struct multiboot_tag_mmap*)tag)->entry_size))
    {
        memset(e_map, 0, sizeof(struct e820map));
        e_map->base_address = (uint64_t)(mmap->addr & 0xffffffff);
        e_map->length = (uint64_t)(mmap->len & 0xffffffff);
        e_map->type = mmap->type;
        e_map++;
        size++;
    }
    mem_map.block_size = size;
}

struct mem_map* get_memory_map()
{
    return &mem_map;
}

void get_memory_info()
{
    struct mem_map* mem_map = get_memory_map();
    assert(mem_map->block_size != 0);
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

void unpack_multiboot()
{
    extern uint32_t multiboot_magic;
    extern struct multiboot_info* multiboot_info;
    assert(multiboot_magic == MULTIBOOT2_BOOTLOADER_MAGIC);

    struct multiboot_tag* tag;
    for (tag = (struct multiboot_tag*)multiboot_info->tags;
        tag->type != MULTIBOOT_TAG_TYPE_END;
        tag = (struct multiboot_tag*)((multiboot_uint8_t*)tag
            + ((tag->size + 7) & ~7)))
    {
        switch (tag->type)
        {
        case MULTIBOOT_TAG_TYPE_MMAP:
        {
            init_memory_map((struct multiboot_tag_mmap*)tag);
        }
        break;
        }
    }
}

int memcmp(void* s1, void* s2, int count)
{
    char* c1 = s1;
    char* c2 = s2;
    while (count-- > 0)
    {
        if (*c1++ != *c2++)
        {
            return c1[-1] < c2[-1] ? -1 : 1;
        }
    }

    return 0;
}