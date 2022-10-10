#include "lib.h"

int main(int argc, char** argv)
{
    int i = 0;
    void* ptr = malloc(4096); // 4k 0
    void* ptr1 = malloc(7 * 4096); // 4k 1
    void* ptr2 = malloc(2 * 4096); // 4k 0
    void* ptr3 = malloc(6 * 4096); // 4k 2
    char* str = (char*)ptr;
    char* str1 = (char*)ptr1;
    char* str2 = (char*)ptr2;
    char* str3 = (char*)ptr3;
    while (1)
    {
        sleep(1);
        printf("process1 hello, %d\n", i);
        *str = 'A';
        *str1 = 'B';
        *str2 = 'C';
        *str3 = 'D';
        str++; str1++; str2++; str3++;
        if (i > 10)
            break;
        i++;
    }
    *str = 0;
    printf("process1 %s\n", (char*)ptr);
    printf("process1 %s\n", (char*)ptr1);
    printf("process1 %s\n", (char*)ptr2);
    printf("process1 %s\n", (char*)ptr3);
    free(ptr3);
    free(ptr);
    free(ptr2);
    free(ptr1);
    return 0;
}