#ifndef MULTIBOOT_INFO_H
#define MULTIBOOT_INFO_H
#include "multiboot/multiboot.h"

struct multiboot_info
{
    multiboot_uint32_t size;
    multiboot_uint32_t reserved;
    struct multiboot_tag tags[];
};


#endif