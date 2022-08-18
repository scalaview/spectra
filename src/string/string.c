#include "string.h"

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