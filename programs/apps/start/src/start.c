#include "lib.h"

int main(int argc, char** argv)
{
    int i = 0;
    while (1)
    {
        if (i % 1000000 == 0)
            printf("process starts, %d\n", i);
        if (i > 30000000)
            break;
        i++;
    }
    return 0;
}