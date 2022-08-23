#ifndef STRING_H
#define STRING_H

#include <stdint.h>

char* itoa(int64_t i, char* text);
int strncmp(const char* str1, const char* str2, int n);
int strlen(const char* ptr);

#endif