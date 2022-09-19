#ifndef DISK_H
#define DISK_H
#include "stddef.h"

typedef unsigned int OS_DIS_TYPE;
#define OS_DISK_TYPE_REAL 0

struct disk
{
    OS_DIS_TYPE type;
    int sector_size;
    // The id of the disk
    int id;

    struct filesystem* filesystem;
    void* fd;
};

void disk_search_and_initialize();
struct disk* get_disk(int index);
int disk_read_sector(int lba, int total, void* buffer);
int disk_read_block(struct disk* idisk, unsigned lba, size_t total, void* buffer);

#endif
