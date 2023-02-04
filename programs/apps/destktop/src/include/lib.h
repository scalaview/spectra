#ifndef _HELLO_LIB_H_
#define _HELLO_LIB_H_

#include <stddef.h>
#include <stdint.h>

int printf(const char* format, ...);
int sleep(int seconds);
void exit(int status);
void* malloc(size_t size);
void free(void* ptr);
extern void* create_window_content(int64_t x, int64_t y, uint32_t width, uint32_t height, uint32_t gcolor);

#endif