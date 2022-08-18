#include "disk.h"
#include <elf.h>

void bootmain()
{
    struct Elf64_Ehdr* buf = (struct Elf64_Ehdr*)0x01000000;
    disk_read_sector(5, 1, (void*)buf);
    while (1) {}
}