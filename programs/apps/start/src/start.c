#include "lib.h"

int main(int argc, char** argv)
{
    int i = 0;
    printf("fork start\n");
    int pid = fork();
    printf("fork end %d\n", pid);

    while (1)
    {
        if (i % 100000000 == 0)
            printf("process starts , %d\n", i);
        i++;
    }

    return 0;
}