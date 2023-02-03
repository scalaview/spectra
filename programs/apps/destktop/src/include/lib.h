#ifndef _HELLO_LIB_H_
#define _HELLO_LIB_H_

#include <stddef.h>

int printf(const char* format, ...);
int sleep(int seconds);
void exit(int status);
void* malloc(size_t size);
void free(void* ptr);

#endif