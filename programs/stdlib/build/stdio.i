# 1 "src/stdio.c"
# 1 "/home/zbin/spectra/programs/stdlib//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "src/stdio.c"
# 1 "/home/zbin/opt/cross_64/lib/gcc/x86_64-elf/10.2.0/include/stdint.h" 1 3 4
# 11 "/home/zbin/opt/cross_64/lib/gcc/x86_64-elf/10.2.0/include/stdint.h" 3 4
# 1 "/home/zbin/opt/cross_64/lib/gcc/x86_64-elf/10.2.0/include/stdint-gcc.h" 1 3 4
# 34 "/home/zbin/opt/cross_64/lib/gcc/x86_64-elf/10.2.0/include/stdint-gcc.h" 3 4

# 34 "/home/zbin/opt/cross_64/lib/gcc/x86_64-elf/10.2.0/include/stdint-gcc.h" 3 4
typedef signed char int8_t;


typedef short int int16_t;


typedef int int32_t;


typedef long int int64_t;


typedef unsigned char uint8_t;


typedef short unsigned int uint16_t;


typedef unsigned int uint32_t;


typedef long unsigned int uint64_t;




typedef signed char int_least8_t;
typedef short int int_least16_t;
typedef int int_least32_t;
typedef long int int_least64_t;
typedef unsigned char uint_least8_t;
typedef short unsigned int uint_least16_t;
typedef unsigned int uint_least32_t;
typedef long unsigned int uint_least64_t;



typedef int int_fast8_t;
typedef int int_fast16_t;
typedef int int_fast32_t;
typedef long int int_fast64_t;
typedef unsigned int uint_fast8_t;
typedef unsigned int uint_fast16_t;
typedef unsigned int uint_fast32_t;
typedef long unsigned int uint_fast64_t;




typedef long int intptr_t;


typedef long unsigned int uintptr_t;




typedef long int intmax_t;
typedef long unsigned int uintmax_t;
# 12 "/home/zbin/opt/cross_64/lib/gcc/x86_64-elf/10.2.0/include/stdint.h" 2 3 4
# 2 "src/stdio.c" 2
# 1 "/home/zbin/opt/cross_64/lib/gcc/x86_64-elf/10.2.0/include/stdarg.h" 1 3 4
# 40 "/home/zbin/opt/cross_64/lib/gcc/x86_64-elf/10.2.0/include/stdarg.h" 3 4
typedef __builtin_va_list __gnuc_va_list;
# 99 "/home/zbin/opt/cross_64/lib/gcc/x86_64-elf/10.2.0/include/stdarg.h" 3 4
typedef __gnuc_va_list va_list;
# 3 "src/stdio.c" 2

# 1 "./src/include/stdio.h" 1






# 6 "./src/include/stdio.h"
int printf(const char* format, ...);
extern int sys_print(const char* buffer, int size, char color);
# 5 "src/stdio.c" 2
# 1 "./src/include/memory.h" 1



# 1 "/home/zbin/opt/cross_64/lib/gcc/x86_64-elf/10.2.0/include/stddef.h" 1 3 4
# 143 "/home/zbin/opt/cross_64/lib/gcc/x86_64-elf/10.2.0/include/stddef.h" 3 4

# 143 "/home/zbin/opt/cross_64/lib/gcc/x86_64-elf/10.2.0/include/stddef.h" 3 4
typedef long int ptrdiff_t;
# 209 "/home/zbin/opt/cross_64/lib/gcc/x86_64-elf/10.2.0/include/stddef.h" 3 4
typedef long unsigned int size_t;
# 321 "/home/zbin/opt/cross_64/lib/gcc/x86_64-elf/10.2.0/include/stddef.h" 3 4
typedef int wchar_t;
# 5 "./src/include/memory.h" 2


# 6 "./src/include/memory.h"
void* memset(void* ptr, int c, size_t size);
int memcmp(void* s1, void* s2, int count);
void* memcpy(void* dest, void* src, int len);
# 6 "src/stdio.c" 2
# 1 "./src/include/string.h" 1





int strlen(const char* ptr);
char* itoa(int64_t i, char* text);
# 7 "src/stdio.c" 2

int read_string(char* buffer, int position, const char* str)
{
    size_t len = strlen(str);
    for (int i = 0; i < len;i++)
    {
        buffer[position++] = str[i];
        if (str[i] == '\n' || position >= 1024)
        {

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
    
# 75 "src/stdio.c" 3 4
   __builtin_va_start(
# 75 "src/stdio.c"
   args
# 75 "src/stdio.c" 3 4
   ,
# 75 "src/stdio.c"
   format
# 75 "src/stdio.c" 3 4
   )
# 75 "src/stdio.c"
                         ;
    memset(buffer, 0, sizeof(buffer));

    for (p = format; *p; p++)
    {
        if (*p != '%')
        {
            buffer[buffer_size++] = *p;

            continue;
        }
        switch (*++p)
        {
        case 'd':
            interger = 
# 89 "src/stdio.c" 3 4
                      __builtin_va_arg(
# 89 "src/stdio.c"
                      args
# 89 "src/stdio.c" 3 4
                      ,
# 89 "src/stdio.c"
                      int64_t
# 89 "src/stdio.c" 3 4
                      )
# 89 "src/stdio.c"
                                           ;
            buffer_size += decimal_to_string(buffer, buffer_size, interger);
            break;
        case 'x':
            interger = 
# 93 "src/stdio.c" 3 4
                      __builtin_va_arg(
# 93 "src/stdio.c"
                      args
# 93 "src/stdio.c" 3 4
                      ,
# 93 "src/stdio.c"
                      int64_t
# 93 "src/stdio.c" 3 4
                      )
# 93 "src/stdio.c"
                                           ;
            buffer_size += hex_to_string(buffer, buffer_size, interger);
            break;
        case 's':
            string = 
# 97 "src/stdio.c" 3 4
                    __builtin_va_arg(
# 97 "src/stdio.c"
                    args
# 97 "src/stdio.c" 3 4
                    ,
# 97 "src/stdio.c"
                    char*
# 97 "src/stdio.c" 3 4
                    )
# 97 "src/stdio.c"
                                       ;
            buffer_size = read_string(buffer, buffer_size, string);
            break;
        default:
            buffer[buffer_size++] = '%';
            p--;
            break;
        }

    }
    buffer_size = sys_print(buffer, buffer_size, 0xf);
    
# 108 "src/stdio.c" 3 4
   __builtin_va_end(
# 108 "src/stdio.c"
   args
# 108 "src/stdio.c" 3 4
   )
# 108 "src/stdio.c"
               ;
    return buffer_size;
}
