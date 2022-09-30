# 1 "src/memory.c"
# 1 "/home/zbin/spectra/programs/stdlib//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "src/memory.c"
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
# 2 "src/memory.c" 2

void* memset(void* ptr, int c, size_t size)
{
    char* ch_ptr = (char*)ptr;
    for (int i = 0;i < size; i++)
    {
        ch_ptr[i] = (char)c;
    }
    return ptr;
}

void* memcpy(void* dest, void* src, int len)
{
    char* d = dest;
    char* s = src;

    while (len--)
    {
        *d++ = *s++;
    }
    return dest;
}

int memcmp(void* s1, void* s2, int count)
{
    char* c1 = s1;
    char* c2 = s2;
    while (count-- > 0)
    {
        if (*c1++ != *c2++)
        {
            return c1[-1] < c2[-1] ? -1 : 1;
        }
    }

    return 0;
}
