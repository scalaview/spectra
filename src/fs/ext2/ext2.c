#include "fs/ext2.h"
#include "heap/kheap.h"
#include "config.h"
#include "status.h"
#include "stream.h"
#include "string.h"
#include "kmemory.h"

int ext2_resolve(struct disk* disk);
int ext2_open(struct disk* disk, struct path_part* path, FILE_MODE mode, void** ptr);
int ext2_read(struct disk* disk, void* descriptor, uint32_t size, uint32_t nmemb, char* out_ptr);
int ext2_seek(void* fd, uint32_t offset, FILE_SEEK_MODE seek_mode);
int ext2_stat(struct disk* disk, void* fd, struct file_stat* stat);
int ext2_close(void* fd);

struct filesystem ext2_fs =
{
    .resolve = ext2_resolve,
    .open = ext2_open,
    .read = ext2_read,
    .seek = ext2_seek,
    .stat = ext2_stat,
    .close = ext2_close
};

static int ext2_initialize_stream(struct disk* idisk, struct ext2_stream* ext2_stream)
{
    int res = 0;
    ext2_stream->cluster_read_stream = create_disk_streamer(idisk->id);
    ext2_stream->directory_stream = create_disk_streamer(idisk->id);
    if (!ext2_stream->cluster_read_stream || !ext2_stream->directory_stream)
    {
        res = -ENOMEM;
        goto out;
    }
    disk_streamer_seek(ext2_stream->cluster_read_stream, MBR_END_POSITION);
    disk_streamer_seek(ext2_stream->directory_stream, MBR_END_POSITION);
out:
    return res;
}

static int read_ext2_superblock(struct disk_stream* stream, struct ext2_superblock* superblock)
{
    int res = 0;
    disk_streamer_seek(stream, MBR_END_POSITION + EXT2_SUPERBLOCK_OFFSET);

    res = disk_streamer_read(stream, superblock, sizeof(struct ext2_superblock));
    if (res <= 0)
    {
        goto out;
    }
    if (superblock->magic != EXT2_SUPER_MAGIC)
    {
        res = -EFSNOTUS;
        memset(superblock, 0, sizeof(struct ext2_superblock));
    }
out:
    return res;
}

static int ext2_get_block_group_descriptor(struct disk_stream* stream, uint32_t block_size, struct ext2_block_group_descriptor* group_desc)
{
    disk_streamer_seek(stream, MBR_END_POSITION + block_size);
    return disk_streamer_read(stream, group_desc, sizeof(struct ext2_block_group_descriptor));
}

static int read_inode(uint32_t block_size, uint32_t inode_no, struct disk_stream* stream, struct ext2_block_group_descriptor* group_desc, struct ext2_inode* inode)
{
    uint32_t offset = BLOCK_OFFSET(group_desc->inode_table, block_size) + (inode_no - 1) * sizeof(struct ext2_inode);
    disk_streamer_seek(stream, offset);
    return disk_streamer_read(stream, inode, sizeof(struct ext2_inode));
}

static int read_root_directory(uint32_t block_size, struct disk_stream* stream, struct ext2_block_group_descriptor* group_desc, struct ext2_inode* root_inode)
{
    return read_inode(block_size, EXT2_ROOT_INODE_NO, stream, group_desc, root_inode);
}

static int ext2_get_root_directory(struct disk_stream* stream, struct ext2_stream* ext2_stream)
{
    int res = ext2_get_block_group_descriptor(stream, ext2_stream->block_size, &ext2_stream->group_descriptor);
    if (res <= 0)
    {
        goto out;
    }
    struct ext2_block_group_descriptor* group_desc = &ext2_stream->group_descriptor;
    res = read_root_directory(ext2_stream->block_size, stream, group_desc, &ext2_stream->root_directory);
    if (res <= 0)
    {
        goto out;
    }
    if (!S_ISDIR(ext2_stream->root_directory.mode))
    {
        res = -EFSNOTUS;

        goto out;
    }
out:
    return res;
}


static struct ext2_inode* ext2_find_inode_in_directory(struct disk* idisk, struct ext2_inode* dir_inode, const char* path)
{
    struct ext2_inode* target = 0;
    int res = 0;
    struct ext2_stream* ext2_stream = idisk->fd;
    struct disk_stream* stream = ext2_stream->directory_stream;
    struct ext2_dir_entry_2* entry = kzalloc(ext2_stream->block_size);
    if (!entry)
    {
        res = -ENOMEM;
        goto out;
    }
    disk_streamer_seek(stream, BLOCK_OFFSET(dir_inode->block[0], ext2_stream->block_size));
    res = disk_streamer_read(stream, entry, ext2_stream->block_size);

    if (res <= 0)
    {
        goto out;
    }

    int size = 0;
    while ((size < dir_inode->size) && entry->inode) {
        if (!strncmp(path, (char*)entry->name, strlen(path)))
        {
            target = kzalloc(sizeof(struct ext2_inode));
            read_inode(ext2_stream->block_size, entry->inode, stream, &ext2_stream->group_descriptor, target);
            break;
        }
        entry = (void*)entry + entry->rec_len;
        size += entry->rec_len;
    }
out:
    kfree(entry);
    return target;
}

static struct ext2_inode* ext2_get_file_inode(struct disk* idisk, struct path_part* path)
{
    struct path_part* next_part = path;
    struct ext2_stream* ext2_stream = idisk->fd;
    struct ext2_inode* current_item = 0;
    while (next_part)
    {
        if (!current_item) //First time
        {
            current_item = &ext2_stream->root_directory;
        }
        current_item = ext2_find_inode_in_directory(idisk, current_item, next_part->path);
        if (!current_item)
        {
            goto out;
        }
        next_part = next_part->next;
    }
out:
    return current_item;
}

int ext2_resolve(struct disk* disk)
{
    int res = 0;
    struct ext2_stream* ext2_stream = kzalloc(sizeof(struct ext2_stream));
    ext2_initialize_stream(disk, ext2_stream);

    disk->fd = ext2_stream;
    disk->filesystem = &ext2_fs;

    struct disk_stream* stream = create_disk_streamer(disk->id);
    if (!stream)
    {
        res = -ENOMEM;
        goto out;
    }

    res = read_ext2_superblock(stream, &ext2_stream->header);
    if (res <= 0)
    {
        goto out;
    }
    struct ext2_superblock* superblock = &ext2_stream->header;
    ext2_stream->block_size = EXT2_MIN_SUPERBLOCK_SIZE << superblock->log_block_size;

    res = ext2_get_root_directory(stream, ext2_stream);
out:
    if (res <= 0)
    {
        disk_streamer_close(ext2_stream->cluster_read_stream);
        disk_streamer_close(ext2_stream->directory_stream);
        kfree(ext2_stream);
        disk->fd = 0;
    }
    if (stream)
        disk_streamer_close(stream);
    return res;
}


int ext2_open(struct disk* disk, struct path_part* path, FILE_MODE mode, void** ptr)
{
    int res = 0;
    if (mode != FILE_MODE_READ) // Only support read now
    {
        res = -EINVARG;
        goto out;
    }
    struct ext2_file_descriptor* descriptor = kzalloc(sizeof(struct ext2_file_descriptor));
    if (!descriptor)
    {
        res = -ENOMEM;
        goto out;
    }
    descriptor->inode = ext2_get_file_inode(disk, path);
    if (!descriptor->inode)
    {
        res = -EIO;
        goto out;
    }
    descriptor->position = 0;
    *ptr = descriptor;
out:
    return res;
}

int ext2_read(struct disk* disk, void* descriptor, uint32_t size, uint32_t nmemb, char* out_ptr)
{
    return 0;
}

int ext2_seek(void* fd, uint32_t offset, FILE_SEEK_MODE seek_mode)
{
    return 0;
}

int ext2_stat(struct disk* disk, void* fd, struct file_stat* stat)
{
    return 0;
}

int ext2_close(void* fd)
{
    return 0;
}

struct filesystem* ext2_initialize()
{
    strcpy(ext2_fs.name, "EXT2");
    return &ext2_fs;
}