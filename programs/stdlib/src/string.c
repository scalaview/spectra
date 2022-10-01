#include "string.h"

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