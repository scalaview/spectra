#include "lib.h"

int main(int argc, char** argv)
{
    int i = 0;
    void* ptr = malloc(4096); // 4k 0
    char* str = (char*)ptr;
    *str = 'O';
    printf("fork start\n");
    int pid = fork();
    printf("fork end %d\n", pid);
    printf("*str: %s\n", str);
    *str = 'C';
    printf("*str: %s\n", str);
    if (argc > 0)
    {
        printf("recieve argv:\n");
        for (int j = 0;j < argc;j++)
        {
            sleep(2);
            printf("recieve argv%d: %s\n", j, argv[j]);
        }
    }
    while (1)
    {
        if (i % 100000000 == 0)
            printf("process starts , %d\n", i);
        i++;
    }
    free(ptr);
    return 0;
}