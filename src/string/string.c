#include <stdint.h>
#include "string.h"
#include <stdbool.h>

char* itoa(int64_t i, char* text)
{
    // static char text[20];
    int position = 19;
    text[position] = 0;
    char negative = 1;
    if (i >= 0)
    {
        negative = 0;
        i = -i;
    }
    while (i)
    {
        text[--position] = '0' - (i % 10);
        i /= 10;
    }

    if (position == 19)
        text[--position] = '0';

    if (negative)
        text[--position] = '-';

    return &text[position];
}

int strncmp(const char* str1, const char* str2, int n)
{
    unsigned char u1, u2;
    while (n-- > 0)
    {
        u1 = (unsigned char)*str1++;
        u2 = (unsigned char)*str2++;
        if (u1 != u2)
            return u1 - u2;
        if (u1 == '\0')
            return 0;
    }
    return 0;
}

int strlen(const char* ptr)
{
    int len = 0;
    while (*ptr != 0)
    {
        len++;
        ptr += 1;
    }

    return len;
}

int strnlen(const char* ptr, int max)
{
    int i = 0;
    for (i = 0; i < max; i++)
    {
        if (ptr[i] == 0)
            break;
    }
    return i;
}

bool isdigit(char c)
{
    return c >= 48 && c <= 57;
}

int to_numeric(char c)
{
    return c - 48;
}

char* strcpy(char* dest, const char* src)
{
    char* res = dest;
    while (*src != 0)
    {
        *dest = *src;
        src += 1;
        dest += 1;
    }
    dest = 0x00;
    return res;
}
