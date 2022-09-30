# 1 "src/string.c"
# 1 "/home/zbin/spectra/programs/stdlib//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "src/string.c"
# 1 "./src/include/string.h" 1



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
# 5 "./src/include/string.h" 2


# 6 "./src/include/string.h"
int strlen(const char* ptr);
char* itoa(int64_t i, char* text);
# 2 "src/string.c" 2

char* itoa(int64_t i, char* text)
{

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
