#include "fs/ext2.h"
#include "heap/kheap.h"
#include "config.h"
#include "status.h"
#include "stream.h"
#include "string.h"
#include "kmemory.h"

int ext2_resolve(struct disk* idisk);
int ext2_open(struct disk* idisk, struct path_part* path, FILE_MODE mode, void** ptr);
int ext2_read(struct disk* idisk, void* fd, uint32_t size, uint32_t nmemb, char* out_ptr);
int ext2_seek(void* fd, uint32_t offset, FILE_SEEK_MODE seek_mode);
int ext2_stat(struct disk* idisk, void* fd, struct file_stat* stat);
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

static int ext2_initialize_stream(struct disk* idisk, struct ext2_fs_descriptor* ext2_fs_descriptor)
{
    int res = 0;
    ext2_fs_descriptor->cluster_read_stream = create_disk_streamer(idisk->id);
    ext2_fs_descriptor->directory_stream = create_disk_streamer(idisk->id);
    if (!ext2_fs_descriptor->cluster_read_stream || !ext2_fs_descriptor->directory_stream)
    {
        res = -ENOMEM;
        goto out;
    }
    disk_streamer_seek(ext2_fs_descriptor->cluster_read_stream, MBR_END_POSITION);
    disk_streamer_seek(ext2_fs_descriptor->directory_stream, MBR_END_POSITION);
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

static int ext2_get_root_directory(struct disk_stream* stream, struct ext2_fs_descriptor* ext2_fs_descriptor)
{
    int res = ext2_get_block_group_descriptor(stream, ext2_fs_descriptor->block_size, &ext2_fs_descriptor->group_descriptor);
    if (res <= 0)
    {
        goto out;
    }
    struct ext2_block_group_descriptor* group_desc = &ext2_fs_descriptor->group_descriptor;
    res = read_root_directory(ext2_fs_descriptor->block_size, stream, group_desc, &ext2_fs_descriptor->root_directory);
    if (res <= 0)
    {
        goto out;
    }
    if (!S_ISDIR(ext2_fs_descriptor->root_directory.mode))
    {
        res = -EFSNOTUS;

        goto out;
    }
out:
    return res;
}

static int ext2_read_block(struct disk* idisk, uint32_t offset, void* out_buffer)
{
    struct ext2_fs_descriptor* ext2_fs_descriptor = idisk->fd;
    struct disk_stream* stream = ext2_fs_descriptor->directory_stream;
    disk_streamer_seek(stream, BLOCK_OFFSET(offset, ext2_fs_descriptor->block_size));
    int res = disk_streamer_read(stream, out_buffer, ext2_fs_descriptor->block_size);
    return res;
}

static struct ext2_inode* ext2_find_inode_in_directory(struct disk* idisk, struct ext2_inode* dir_inode, const char* path)
{
    struct ext2_inode* target = 0;
    int res = 0;
    struct ext2_fs_descriptor* ext2_fs_descriptor = idisk->fd;
    struct disk_stream* stream = ext2_fs_descriptor->directory_stream;
    struct ext2_dir_entry_2* entry = kzalloc(ext2_fs_descriptor->block_size);
    if (!entry)
    {
        res = -ENOMEM;
        goto out;
    }
    if (!dir_inode->size)
    {
        res = -EFNOTFOUND;
        goto out;
    }
    res = ext2_read_block(idisk, dir_inode->block[0], entry);
    if (res <= 0)
    {
        goto out;
    }

    int size = 0;
    struct ext2_dir_entry_2* t_entry = entry;
    while ((size < dir_inode->size) && t_entry->inode) {
        if (!strncmp(path, (char*)t_entry->name, strlen(path)))
        {
            target = kzalloc(sizeof(struct ext2_inode));
            read_inode(ext2_fs_descriptor->block_size, t_entry->inode, stream, &ext2_fs_descriptor->group_descriptor, target);
            break;
        }
        t_entry = (void*)t_entry + t_entry->rec_len;
        size += t_entry->rec_len;
    }
out:
    kfree(entry);
    return target;
}

static struct ext2_inode* ext2_get_file_inode(struct disk* idisk, struct path_part* path)
{
    struct path_part* next_part = path;
    struct ext2_fs_descriptor* ext2_fs_descriptor = idisk->fd;
    struct ext2_inode* current_item = 0;
    while (next_part)
    {
        if (!current_item) //First time
        {
            current_item = &ext2_fs_descriptor->root_directory;
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

int ext2_resolve(struct disk* idisk)
{
    int res = 0;
    struct ext2_fs_descriptor* ext2_fs_descriptor = kzalloc(sizeof(struct ext2_fs_descriptor));
    ext2_initialize_stream(idisk, ext2_fs_descriptor);

    idisk->fd = ext2_fs_descriptor;
    idisk->filesystem = &ext2_fs;

    struct disk_stream* stream = create_disk_streamer(idisk->id);
    if (!stream)
    {
        res = -ENOMEM;
        goto out;
    }

    res = read_ext2_superblock(stream, &ext2_fs_descriptor->header);
    if (res <= 0)
    {
        goto out;
    }
    struct ext2_superblock* superblock = &ext2_fs_descriptor->header;
    ext2_fs_descriptor->block_size = EXT2_MIN_SUPERBLOCK_SIZE << superblock->log_block_size;

    res = ext2_get_root_directory(stream, ext2_fs_descriptor);
out:
    if (res <= 0)
    {
        disk_streamer_close(ext2_fs_descriptor->cluster_read_stream);
        disk_streamer_close(ext2_fs_descriptor->directory_stream);
        kfree(ext2_fs_descriptor);
        idisk->fd = 0;
    }
    if (stream)
        disk_streamer_close(stream);
    return res;
}


int ext2_open(struct disk* idisk, struct path_part* path, FILE_MODE mode, void** ptr)
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
    descriptor->inode = ext2_get_file_inode(idisk, path);
    if (!descriptor->inode)
    {
        res = -EFNOTFOUND;
        goto out;
    }
    descriptor->position = 0;
    *ptr = descriptor;
out:
    return res;
}

int ext2_read_indirect(struct disk* idisk, struct ext2_fs_descriptor* ext2_fs_descriptor, int32_t* num_blocks_cache, int left_size, uint32_t indirect, size_t block_num) {
    int res = 0;
    if (block_num >= (ext2_fs_descriptor->block_size / 4))
    {
        res = -EINVARG;
        goto out;
    }
    int32_t* indirect_array = kzalloc(ext2_fs_descriptor->block_size);
    if (!indirect_array)
    {
        res = -ENOMEM;
        goto out;
    }
    res = ext2_read_block(idisk, indirect, indirect_array);
    if (res <= 0)
    {
        goto out;
    }
    if (num_blocks_cache && left_size)
    {
        int ind_size = ext2_fs_descriptor->block_size / sizeof(int32_t);
        memcpy(num_blocks_cache, indirect_array, (sizeof(int32_t) * (left_size > ind_size ? ind_size : left_size)));
    }
    res = *(indirect_array + block_num);
out:
    kfree(indirect_array);
    return res;
}

static int __ext2_read_block_num(struct disk* idisk, struct ext2_fs_descriptor* ext2_fs_descriptor, struct ext2_inode* desc_inode, size_t blocks_size, int block_i, int32_t* num_blocks)
{
    int blocknum = *(num_blocks + block_i);
    if (blocknum > 0)   return blocknum;

    int level_0_block_range = ext2_fs_descriptor->block_size / 4;
    int level_1_block_range = level_0_block_range * level_0_block_range;
    int level_2_block_range = level_0_block_range * level_0_block_range * level_0_block_range;
    if (block_i < EXT2_IND_BLOCK)
    {
        blocknum = desc_inode->block[block_i];
        memcpy(num_blocks, desc_inode->block, (sizeof(int32_t) * EXT2_IND_BLOCK));
    }
    else if (block_i >= EXT2_IND_BLOCK && block_i < EXT2_IND_BLOCK + level_0_block_range)
    {
        blocknum = ext2_read_indirect(idisk, ext2_fs_descriptor, num_blocks + EXT2_IND_BLOCK, blocks_size - EXT2_IND_BLOCK, desc_inode->block[EXT2_IND_BLOCK], block_i - EXT2_IND_BLOCK);
    }
    else if (block_i >= EXT2_IND_BLOCK + level_0_block_range && block_i < EXT2_IND_BLOCK + level_0_block_range + level_1_block_range)
    {
        int j = block_i - (EXT2_IND_BLOCK + level_0_block_range);
        int indirect_level_1 = j / level_0_block_range;
        int indirect_level_2 = j % level_0_block_range;

        int indirect = ext2_read_indirect(idisk, ext2_fs_descriptor, 0, 0, desc_inode->block[EXT2_DIND_BLOCK], indirect_level_1);
        blocknum = ext2_read_indirect(idisk, ext2_fs_descriptor, num_blocks + (block_i - indirect_level_2), blocks_size - block_i + indirect_level_2, indirect, indirect_level_2);
    }
    else if (block_i >= EXT2_IND_BLOCK + level_0_block_range + level_1_block_range && block_i < EXT2_IND_BLOCK + level_0_block_range + level_1_block_range + level_2_block_range)
    {
        int j = block_i - (EXT2_IND_BLOCK + level_0_block_range + level_1_block_range);
        int indirect_level_1 = j / level_1_block_range;
        int indirect_level_2 = j % level_1_block_range;

        int indirect_level_3 = indirect_level_2 / level_0_block_range;
        int indirect_level_4 = indirect_level_2 % level_0_block_range;

        int indirect_1 = ext2_read_indirect(idisk, ext2_fs_descriptor, 0, 0, desc_inode->block[EXT2_TIND_BLOCK], block_i - indirect_level_1);
        int indirect_2 = ext2_read_indirect(idisk, ext2_fs_descriptor, 0, 0, indirect_1, indirect_level_3);
        blocknum = ext2_read_indirect(idisk, ext2_fs_descriptor, num_blocks + (block_i - indirect_level_4), blocks_size - block_i + indirect_level_4, indirect_2, indirect_level_4);
    }
    else
    {
        blocknum = -1;
    }

    return blocknum;
}

int ext2_read(struct disk* idisk, void* fd, uint32_t size, uint32_t nmemb, char* out_ptr)
{
    int res = 0;
    struct ext2_file_descriptor* descriptor = (struct ext2_file_descriptor*)fd;
    char* t_out_ptr = out_ptr;
    struct ext2_inode* desc_inode = descriptor->inode;

    if (!desc_inode)
    {
        res = -EINVARG;
        goto out;
    }
    struct ext2_fs_descriptor* ext2_fs_descriptor = idisk->fd;
    char* buffer = kzalloc(ext2_fs_descriptor->block_size);
    if (!buffer)
    {
        res = -ENOMEM;
        goto out;
    }
    if (!desc_inode->size)
    {
        res = -EFNOTFOUND;
        goto out;
    }
    int blocks_size = desc_inode->blocks / (ext2_fs_descriptor->block_size / SECTOR_SIZE);
    int32_t* num_blocks = (int32_t*)kzalloc(blocks_size * sizeof(int32_t));
    memset(num_blocks, -1, blocks_size * sizeof(int32_t));
    uint32_t read = 0;
    uint32_t skip = descriptor->position % ext2_fs_descriptor->block_size;
    uint32_t offset = descriptor->position / ext2_fs_descriptor->block_size;
    for (int i = offset; i < blocks_size && read < desc_inode->size && read < size; i++)
    {
        uint32_t left = size - read;
        int blocknum = __ext2_read_block_num(idisk, ext2_fs_descriptor, desc_inode, blocks_size, i, num_blocks);
        if (blocknum < 0)
        {
            res = -EIO;
            goto out;
        }
        res = ext2_read_block(idisk, blocknum, buffer);
        if (res <= 0)
        {
            goto out;
        }
        size_t read_size = left > ext2_fs_descriptor->block_size ? (ext2_fs_descriptor->block_size - skip) : left;
        memcpy(t_out_ptr, buffer + skip, read_size);
        skip = 0;
        t_out_ptr += read_size;
        read += read_size;
    }
out:
    return res;
}

int ext2_seek(void* fd, uint32_t offset, FILE_SEEK_MODE seek_mode)
{
    int res = 0;
    struct ext2_file_descriptor* descriptor = (struct ext2_file_descriptor*)fd;
    struct ext2_inode* desc_inode = descriptor->inode;

    if (!S_ISREG(desc_inode->mode))
    {
        res = -EINVARG;
        goto out;
    }
    if (offset >= desc_inode->size)
    {
        res = -EIO;
        goto out;
    }
    switch (seek_mode)
    {
    case SEEK_SET:
        descriptor->position = offset;
        break;
    case SEEK_END:
        descriptor->position = desc_inode->size;
        break;
    case SEEK_CUR:
        if (descriptor->position + offset >= desc_inode->size)
        {
            res = -EIO;
            break;
        }
        descriptor->position += offset;
        break;
    default:
        res = -EINVARG;
        break;
    }
out:
    return res;
}

int ext2_stat(struct disk* idisk, void* fd, struct file_stat* stat)
{
    int res = 0;
    struct ext2_file_descriptor* descriptor = (struct ext2_file_descriptor*)fd;
    struct ext2_inode* desc_inode = descriptor->inode;
    if (!desc_inode)
    {
        res = -EINVARG;
        goto out;
    }
    stat->atime = desc_inode->atime;
    stat->ctime = desc_inode->ctime;
    stat->dtime = desc_inode->dtime;
    stat->filesize = desc_inode->size;
    stat->mode = desc_inode->mode;
    stat->mtime = desc_inode->mtime;
out:
    return res;
}

static void ext2_free_inode(struct ext2_inode* inode)
{
    kfree(inode);
}

int ext2_close(void* fd)
{
    struct ext2_file_descriptor* descriptor = (struct ext2_file_descriptor*)fd;
    ext2_free_inode(descriptor->inode);
    kfree(descriptor);
    return 0;
}

struct filesystem* ext2_initialize()
{
    strcpy(ext2_fs.name, "EXT2");
    return &ext2_fs;
}