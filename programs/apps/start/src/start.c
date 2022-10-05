#include "lib.h"

int main(int argc, char** argv)
{
    int i = 0;
    if (i)
    {
        i++;
    }
    printf("process starts, %x\n", &i);
    return 0;
}