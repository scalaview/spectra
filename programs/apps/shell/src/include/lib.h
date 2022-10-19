#ifndef _START_LIB_H_
#define _START_LIB_H_

#include <stddef.h>

int printf(const char* format, ...);
int sleep(int seconds);
int fork();
void* malloc(size_t size);
void free(void* ptr);
char keyboard_getkey();

#endif