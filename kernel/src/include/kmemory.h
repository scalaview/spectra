#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdint.h>

#include "config.h"
#include "multiboot/multiboot_info.h"

/**
* 00h    QWORD   base address
* 08h    QWORD   length in bytes
* 10h    DWORD   type of address range (see #00581)
*/
struct e820map
{
    uint64_t base_address;
    uint64_t length;
    uint32_t type;
} __attribute__((packed));

struct mem_map
{
    uint32_t block_size;
    struct e820map* map;
} __attribute__((packed));

#define phy2vir(p) ((uint64_t)(p) + KERNEL_VMA)
#define vir2phy(p) (((uint64_t)(p) != 0) ? ((uint64_t)(p) - KERNEL_VMA) : (uint64_t)(p))

void* memset(void* ptr, int c, size_t size);
int memcmp(void* s1, void* s2, int count);
void get_memory_info();
void init_memory_map(struct multiboot_tag_mmap* tag);
struct mem_map* get_memory_map();
// https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html
void unpack_multiboot();
extern void memcpy(void* dest, void* src, size_t size);

#endif