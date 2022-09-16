#ifndef PATH_H
#define PATH_H

#define OS_MAX_PATH 128

struct path_root
{
    int drive_no;
    struct path_part* root;
};

struct path_part
{
    const char* path;
    struct path_part* next;
};

struct path_root* path_parse(const char* path);
void path_parser_free(struct path_root* root);

#endif