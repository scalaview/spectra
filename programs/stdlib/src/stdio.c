#include <stdint.h>
#include <stdarg.h>

#include "stdio.h"
#include "memory.h"
#include "string.h"

int read_string(char* buffer, int position, const char* str)
{
    size_t len = strlen(str);
    for (int i = 0; i < len;i++)
    {
        buffer[position++] = str[i];
        if (str[i] == '\n' || position >= BUFFER_SIZE)
        {
            // Flush to screen
            sys_print(buffer, position, 0xf);
            position = 0;
        }

    }
    return position;
}

int hex_to_string(char* buffer, int position, int64_t i)
{
    static char hex[16] = "0123456789ABCDEF";
    char text[21];
    memset(text, 0, sizeof(text));
    char negative = 1;
    int size = 0;

    if (i >= 0)
    {
        negative = 0;
    }
    text[size++] = 'H';

    do {
        text[size++] = hex[i % 16];
        i /= 16;
    } while (i != 0);

    if (negative)
        text[size++] = '-';

    for (int i = size - 1; i >= 0; i--) {
        buffer[position++] = text[i];
    }
    return size;
}

int decimal_to_string(char* buffer, int position, int64_t digits)
{
    char text[20];
    char* c = itoa(digits, text);
    int size = 0;

    for (; *c; c++)
    {
        buffer[position++] = *c;
        size++;
    }
    return size;
}

int printf(const char* format, ...)
{
    va_list args;
    char buffer[1024];
    int buffer_size = 0;
    int64_t interger = 0;
    char* string = 0;
    const char* p = 0;
    va_start(args, format);
    memset(buffer, 0, sizeof(buffer));

    for (p = format; *p; p++)
    {
        if (*p != '%')
        {
            buffer[buffer_size++] = *p;
            // assert(buffer_size < 1024);
            continue;
        }
        switch (*++p)
        {
        case 'd':
            interger = va_arg(args, int64_t);
            buffer_size += decimal_to_string(buffer, buffer_size, interger);
            break;
        case 'x':
            interger = va_arg(args, int64_t);
            buffer_size += hex_to_string(buffer, buffer_size, interger);
            break;
        case 's':
            string = va_arg(args, char*);
            buffer_size = read_string(buffer, buffer_size, string);
            break;
        default:
            buffer[buffer_size++] = '%';
            p--;
            break;
        }
        // assert(buffer_size < 1024);
    }
    buffer_size = sys_print(buffer, buffer_size, 0xf);
    va_end(args);
    return buffer_size;
}