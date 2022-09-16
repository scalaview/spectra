#include <stdbool.h>
#include "path.h"
#include "string.h"
#include "heap/kheap.h"
#include "status.h"
#include "kmemory.h"

static bool valid_path(const char* filename)
{
    int len = strnlen(filename, OS_MAX_PATH);
    return (len >= 3 && isdigit(filename[0]) && memcmp((void*)&filename[1], ":/", 2) == 0);
}

static int get_drive_by_path(const char** path)
{
    if (!valid_path(*path))
    {
        return -EBADPATH;
    }
    int drive_no = to_numeric(*path[0]);
    // Add 3 bytes to skip drive number 0:/ 1:/ 2:/
    *path += 3;
    return drive_no;
}

static struct path_root* create_root_path(int drive_no)
{
    struct path_root* root = kzalloc(sizeof(struct path_root));
    root->drive_no = drive_no;
    root->root = 0x0;
    return root;
}

static const char* get_path_part(const char** path)
{
    char* result = kzalloc(OS_MAX_PATH);
    int i = 0;
    while (**path != '/' && **path != 0x0)
    {
        result[i] = **path;
        *path += 1;
        i++;
    }

    if (**path == '/')
    {
        *path += 1;
    }

    if (i == 0)
    {
        kfree(result);
        result = NULL;
    }
    return result;
}

static struct path_part* parse_path_part(struct path_part* last_path, const char** path)
{
    const char* path_part_str = get_path_part(path);
    if (!path_part_str)
    {
        return 0;
    }
    struct path_part* part = kzalloc(sizeof(struct path_part));
    part->path = path_part_str;
    part->next = 0x0;
    if (last_path)
    {
        last_path->next = part;
    }
    return part;
}

struct path_root* path_parse(const char* path)
{
    int res = 0;
    const char* tmp_path = path;
    struct path_root* ipath_root = 0;

    if (strlen(path) > OS_MAX_PATH)
    {
        goto out;
    }

    res = get_drive_by_path(&tmp_path);
    if (res < 0)
    {
        goto out;
    }
    ipath_root = create_root_path(res);
    if (!ipath_root)
    {
        goto out;
    }
    struct path_part* first_path = parse_path_part(NULL, &tmp_path);
    if (!first_path)
    {
        goto out;
    }
    ipath_root->root = first_path;
    struct path_part* part = parse_path_part(first_path, &tmp_path);
    while (part)
    {
        part = parse_path_part(part, &tmp_path);
    }

out:
    return ipath_root;
};

void path_parser_free(struct path_root* root)
{
    struct path_part* part = root->root;
    while (part)
    {
        struct path_part* next_part = part->next;
        kfree((void*)part->path);
        kfree(part);
        part = next_part;
    }
    kfree(root);
}
