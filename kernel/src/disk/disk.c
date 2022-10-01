#include "io.h"
#include "disk.h"
#include "config.h"
#include "file.h"
#include "assert.h"
#include "kmemory.h"
#include "status.h"

struct disk disk;

// ata_lba_read function C implement
int disk_read_sector(int lba, int total, void* buffer)
{
    outb(0x1F6, (lba >> 24) | 0xE0);
    outb(0x1F2, total);
    outb(0x1F3, (unsigned char)(lba & 0xff));
    outb(0x1F4, (unsigned char)(lba >> 8));
    outb(0x1F5, (unsigned char)(lba >> 16));
    outb(0x1F7, 0x20);
    uint32_t res = 0;
    unsigned short* ptr = (unsigned short*)buffer;
    for (int b = 0; b < total; b++)
    {
        // Wait for the buffer to be ready
        char c = insb(0x1F7);
        while (!(c & 0x08))
        {
            c = insb(0x1F7);
        }

        // Copy from hard disk to memory
        for (int i = 0; i < 256; i++)
        {
            *ptr = insw(0x1F0);
            ptr++;
            res++;
        }

    }
    return res;
}

void disk_search_and_initialize()
{
    memset(&disk, 0, sizeof(disk));
    disk.type = OS_DISK_TYPE_REAL;
    disk.sector_size = IO_SECTOR_SIZE;
    disk.id = 0;
    disk.filesystem = fs_resolve(&disk);
    assert(disk.filesystem);
}

struct disk* get_disk(int index)
{
    // Only one disk now
    if (index != 0)
        return 0;

    return &disk;
}

int disk_read_block(struct disk* idisk, unsigned lba, size_t total, void* buffer)
{
    if (idisk != &disk)
    {
        return -EIO;
    }
    return disk_read_sector(lba, total, buffer);
}