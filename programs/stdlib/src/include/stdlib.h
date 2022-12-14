#ifndef OS_STD_LIB_H
#define OS_STD_LIB_H

#include <stddef.h>

#define     RING3           3

#define ARGV_SIZE           256
#define ENVP_SIZE           256

extern void exit(int status);
int wait(int pid);
extern int fork();

int execve(const char* pathname, const char* argv[], const char* envp);
extern void* malloc(size_t size);
extern void free(void* ptr);
#endif