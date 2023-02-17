#include "assets/img/parser.h"
#include "stdio.h"
#include "state.h"
#include "stdlib.h"

int img_tga_parser(const char* file, void** con_ptr)
{
    int res = 0;
    FILE* fd = 0;
    struct file_stat* stat = 0;
    unsigned char* img_ptr = 0;
    fd = fopen(file, "r");
    if (!fd->fdi)
    {
        res = EIO;
        goto out;
    }

    stat = malloc(sizeof(struct file_stat));
    if (!stat)
    {
        res = ENOMEM;
        goto out;
    }
    fstat(fd->fdi, stat);
    img_ptr = malloc(stat->filesize);
    if (!img_ptr)
    {
        res = ENOMEM;
        goto out;
    }
    size_t size = fread(img_ptr, stat->filesize, 1, fd);
    if (!size)
    {
        res = EIO;
        goto out;
    }
    *con_ptr = tga_parse(img_ptr, stat->filesize);

out:
    if (fd)
    {
        fclose(fd);
        free(fd);
    }

    if (stat) free(stat);
    if (res && img_ptr) free(img_ptr);
    return res;
}