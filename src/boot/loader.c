#include "disk.h"

void bootmain()
{
    void* buf = (void*)0x01000000;
    disk_read_sector(5, 1, buf);
    while (1) {}
}