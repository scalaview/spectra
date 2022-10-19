#ifndef OS_STDIO_H
#define OS_STDIO_H

#define BUFFER_SIZE             1024

int printf(const char* format, ...);
extern int sys_print(const char* buffer, int size, char color);
extern char keyboard_getkey();

#endif