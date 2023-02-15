#ifndef OS_STDIO_H
#define OS_STDIO_H

#define BUFFER_SIZE             1024

#include <stdint.h>
#include <stddef.h>


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

struct file_stat
{
    uint16_t mode;
    uint32_t atime;			// Last access time, POSIX
    uint32_t ctime;			// Creation time
    uint32_t mtime;			// Last modified time
    uint32_t dtime;			// Deletion time
    uint32_t filesize;
};

struct io_file
{
    int flag;
    uint32_t fdi;
};

typedef struct io_file FILE;

extern FILE* fopen(const char* filename, const char* mode_str);
extern size_t fread(void* ptr, uint32_t size, uint32_t nmemb, FILE* stream);
extern int fseek(FILE* fd, int offset, FILE_SEEK_MODE mode);
extern int fclose(FILE* stream);
extern int fstat(int fd, struct file_stat* stat);

int printf(const char* format, ...);
extern int sys_print(const char* buffer, int size, char color);
extern char keyboard_getkey();

#endif