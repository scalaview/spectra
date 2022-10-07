#include "lib.h"

int main(int argc, char** argv)
{
    int i = 0;
    while (1)
    {
        sleep(1);
        printf("process1 hello, %d\n", i);
        i++;
    }
    return 0;
}