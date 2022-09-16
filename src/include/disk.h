#ifndef DISK_H
#define DISK_H

typedef unsigned int OS_DIS_TYPE;
#define OS_DISK_TYPE_REAL 0

int disk_read_sector(int lba, int total, void* buffer);

struct disk
{
    OS_DIS_TYPE type;
    int sector_size;
    // The id of the disk
    int id;

    struct filesystem* filesystem;

};

void disk_search_and_initialize();
struct disk* get_disk(int index);

#endif
