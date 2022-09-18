#include "file.h"
#include "config.h"
#include "assert.h"
#include "status.h"
#include "kmemory.h"
#include "heap/kheap.h"
#include "status.h"
#include "string.h"
#include "path.h"
#include "disk.h"
#include "fs/ext2.h"

struct filesystem* filesystems[OS_MAX_FILESYSTEMS];
struct file_descriptor* file_descriptors[OS_MAX_FILE_DESCRIPTORS];

static struct filesystem** get_free_filesystem()
{
    for (int i = 0; i < OS_MAX_FILESYSTEMS; i++)
    {
        if (filesystems[i] == 0)
        {
            return &filesystems[i];
        }
    }
    return 0;
}

void fs_insert_filesystem(struct filesystem* filesystem)
{
    struct filesystem** fs;
    fs = get_free_filesystem();
    assert(fs);
    *fs = filesystem;
}

static void load_filesystem()
{
    fs_insert_filesystem(ext2_initialize());
}

static void fs_load()
{
    memset(filesystems, 0, sizeof(filesystems));
    load_filesystem();
}

void fs_initialize()
{
    memset(file_descriptors, 0, sizeof(file_descriptors));
    fs_load();
}

static void free_file_descriptor(struct file_descriptor* desc)
{
    file_descriptors[desc->index - 1] = 0x00;
    kfree(desc);
}

static int new_file_descriptor(struct file_descriptor** desc_out)
{
    int res = -ENOMEM;
    for (int i = 0; i < OS_MAX_FILE_DESCRIPTORS; i++)
    {
        if (file_descriptors[i])
            continue;
        struct file_descriptor* desc = kzalloc(sizeof(struct file_descriptor));
        desc->index = i + 1;
        file_descriptors[i] = desc;
        *desc_out = desc;
        res = 0;
        break;
    }
    return res;
}

static struct file_descriptor* file_get_descriptor(int fd)
{
    if (fd <= 0 || fd >= OS_MAX_FILE_DESCRIPTORS)
    {
        return 0;
    }

    // Descriptor start at one
    int index = fd - 1;
    return file_descriptors[index];
}

struct filesystem* fs_resolve(struct disk* disk)
{
    struct filesystem* fs = 0;
    for (int i = 0; i < OS_MAX_FILESYSTEMS; i++)
    {
        if (filesystems[i] != 0 && filesystems[i]->resolve(disk) > 0)
        {
            fs = filesystems[i];
            break;
        }
    }
    return fs;
}

FILE_MODE file_get_mode_by_string(const char* str)
{
    FILE_MODE mode = FILE_MODE_INVALID;
    if (strncmp(str, "r", 1) == 0)
    {
        mode = FILE_MODE_READ;
    }
    else if (strncmp(str, "w", 1) == 0)
    {
        mode = FILE_MODE_READ;
    }
    else if (strncmp(str, "a", 1) == 0)
    {
        mode = FILE_MODE_READ;
    }
    return mode;
}

FILE* fopen(const char* filename, const char* mode_str)
{
    int res = 0;
    FILE* fptr = kzalloc(sizeof(FILE));
    struct path_root* root_path = path_parse(filename);
    if (!root_path)
    {
        res = -EINVARG;
        goto out;
    }

    if (!root_path->root)
    {
        res = -EINVARG;
        path_parser_free(root_path);
        goto out;
    }
    struct disk* disk = get_disk(root_path->drive_no);
    if (!disk)
    {
        res = -EIO;
        goto out;
    }

    if (!disk->filesystem)
    {
        res = -EIO;
        goto out;
    }
    FILE_MODE mode = file_get_mode_by_string(mode_str);
    if (mode == FILE_MODE_INVALID)
    {
        res = -EINVARG;
        goto out;
    }
    void* descriptor_data;
    res = disk->filesystem->open(disk, root_path->root, mode, &descriptor_data);
    if (res < 0)
    {
        goto out;
    }
    struct file_descriptor* desc = 0;
    res = new_file_descriptor(&desc);
    if (res < 0)
    {
        goto out;
    }
    desc->filesystem = disk->filesystem;
    desc->stream = descriptor_data;
    desc->disk = disk;
    res = desc->index;

out:
    if (res <= 0)
    {
        fptr->flag = res;
        return fptr;
    }
    fptr->fdi = res;
    return fptr;
}

int fstat(int fd, struct file_stat* statbuf)
{
    int res = 0;
    struct file_descriptor* desc = file_get_descriptor(fd);
    if (!desc)
    {
        res = -EIO;
        goto out;
    }
    res = desc->filesystem->stat(desc->disk, desc->stream, statbuf);
out:
    return res;
}

int fclose(FILE* stream)
{
    int res = 0;
    struct file_descriptor* desc = file_get_descriptor(stream->fdi);
    if (!desc)
    {
        res = -EIO;
        goto out;
    }
    res = desc->filesystem->close(desc->stream);
    if (res == SUCCESS)
    {
        free_file_descriptor(desc);
    }
out:
    return res;
}

int fseek(int fd, int offset, FILE_SEEK_MODE mode)
{
    int res = 0;
    struct file_descriptor* desc = file_get_descriptor(fd);
    if (!desc)
    {
        res = -EIO;
        goto out;
    }

    res = desc->filesystem->seek(desc->stream, offset, mode);
out:
    return res;
}

size_t fread(void* ptr, uint32_t size, uint32_t nmemb, FILE* stream)
{
    int res = 0;
    if (size == 0 || nmemb == 0 || !stream || stream->fdi < 1)
    {
        res = -EINVARG;
        goto out;
    }

    struct file_descriptor* desc = file_get_descriptor(stream->fdi);
    if (!desc)
    {
        res = -EINVARG;
        goto out;
    }

    res = desc->filesystem->read(desc->disk, desc->stream, size, nmemb, (char*)ptr);

out:
    return res;
}