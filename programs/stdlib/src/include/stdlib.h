#ifndef OS_STD_LIB_H
#define OS_STD_LIB_H

#include <stddef.h>

extern void exit(int status);
int wait(int pid);
extern int fork();

#endif