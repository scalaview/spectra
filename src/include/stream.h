#ifndef STREAM_H
#define STREAM_H

#include <stdint.h>
#include <stddef.h>
#include "disk.h"
#include "config.h"

#define STREAM_SECTOR_SIZE(t)      (t >= (4 * IO_SECTOR_SIZE)) ? 2 : 1;

struct disk_stream
{
    uint64_t position;
    struct disk* disk;
};

struct disk_stream* create_disk_streamer(uint32_t disk_id);
int disk_streamer_seek(struct disk_stream* stream, int pos);
size_t disk_streamer_read(struct disk_stream* stream, void* out, int total);
void disk_streamer_close(struct disk_stream* stream);

#endif