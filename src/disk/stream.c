#include "stream.h"
#include "status.h"
#include "heap/kheap.h"
#include "config.h"

struct disk_stream* create_disk_streamer(uint32_t disk_id)
{
    struct disk* disk = get_disk(disk_id);
    if (!disk)
    {
        return 0;
    }
    struct disk_stream* streamer = kzalloc(sizeof(struct disk_stream));
    streamer->disk = disk;
    streamer->position = 0;
    return streamer;
}

/**
 * @brief
 *
 * @param stream
 * @param position
 * @return int should greater tan zero if success
 */
int disk_streamer_seek(struct disk_stream* stream, int position)
{
    stream->position = position;
    return SUCCESS;
}

int disk_streamer_read(struct disk_stream* stream, void* out, int total)
{
    int res = 0;
    uint32_t sector = stream->position / IO_SECTOR_SIZE;
    uint32_t offset = stream->position % IO_SECTOR_SIZE;
    int left = total;
    size_t sector_size = STREAM_SECTOR_SIZE(left);
    char* buffer = kzalloc(sector_size * IO_SECTOR_SIZE);
    if (!buffer)
    {
        res = -ENOMEM;
        goto out;
    }

    while (left > 0)
    {
        res =
            disk_read_block(stream->disk, sector, sector_size, buffer);
        if (res <= 0)
        {
            res = -EIO;
            goto out;
        }
        for (int i = offset; i < (sector_size * IO_SECTOR_SIZE) && left > 0; i++)
        {
            *(char*)out++ = buffer[i];
            left--;
        }
        sector += sector_size;
        offset = 0;
        sector_size = STREAM_SECTOR_SIZE(left);
    }
out:
    kfree(buffer);
    return res;
}

void disk_streamer_close(struct disk_stream* stream)
{
    kfree(stream);
}
