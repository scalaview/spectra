#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stdbool.h>

char* itoa(int64_t i, char* text);
int strncmp(const char* str1, const char* str2, int n);
int strlen(const char* ptr);
bool isdigit(char c);
int strnlen(const char* ptr, int max);
int to_numeric(char c);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, int count);

#endif