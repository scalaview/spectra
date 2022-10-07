#include "lib.h"

int main(int argc, char** argv)
{
    int i = 0;
    while (1)
    {
        if (i % 10000000 == 0)
            printf("process starts, %d\n", i);
        i++;
    }
    return 0;
}