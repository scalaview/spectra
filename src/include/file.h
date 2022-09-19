#ifndef FILE_H
#define FILE_H

#include <stdint.h>
#include <stddef.h>

#include "path.h"

typedef unsigned int FILE_SEEK_MODE;
enum
{
    SEEK_SET,
    SEEK_CUR,
    SEEK_END
};

typedef unsigned int FILE_MODE;
enum
{
    FILE_MODE_READ,
    FILE_MODE_WRITE,
    FILE_MODE_APPEND,
    FILE_MODE_INVALID
};


struct disk;
struct file_stat;
typedef int (*FS_OPEN_FUNCTION)(struct disk* idisk, struct path_part* path, FILE_MODE mode, void** ptr);
typedef int (*FS_READ_FUNCTION)(struct disk* idisk, void* fd, uint32_t size, uint32_t nmemb, char* out);
typedef int (*FS_RESOLVE_FUNCTION)(struct disk* idisk);
typedef int (*FS_CLOSE_FUNCTION)(void* fd);
typedef int (*FS_SEEK_FUNCTION)(void* fd, uint32_t offset, FILE_SEEK_MODE seek_mode);
typedef int (*FS_STAT_FUNCTION)(struct disk* idisk, void* fd, struct file_stat* stat);

struct file_stat
{
    uint16_t mode;
    uint32_t atime;			// Last access time, POSIX
    uint32_t ctime;			// Creation time
    uint32_t mtime;			// Last modified time
    uint32_t dtime;			// Deletion time
    uint32_t filesize;
};

struct filesystem
{
    FS_RESOLVE_FUNCTION resolve;
    FS_OPEN_FUNCTION open;
    FS_READ_FUNCTION read;
    FS_SEEK_FUNCTION seek;
    FS_STAT_FUNCTION stat;
    FS_CLOSE_FUNCTION close;
    char name[20];
};

struct file_descriptor
{
    int index;
    struct filesystem* filesystem;
    void* stream;
    struct disk* disk;
};

struct io_file
{
    int flag;
    uint32_t fdi;
};

typedef struct io_file FILE;

void fs_initialize();
FILE* fopen(const char* filename, const char* mode_str);
size_t fread(void* ptr, uint32_t size, uint32_t nmemb, FILE* stream);
int fseek(FILE* fd, int offset, FILE_SEEK_MODE mode);
int fclose(FILE* stream);
int fstat(int fd, struct file_stat* stat);

void fs_insert_filesystem(struct filesystem* filesystem);
struct filesystem* fs_resolve(struct disk* disk);

#endif