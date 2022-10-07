#include "lib.h"

int main(int argc, char** argv)
{
    execve("0:/usr/bin/start.bin", 0, 0);
    execve("0:/usr/bin/hello.bin", 0, 0);
    while (1)
    {
        wait(0);
    }
    return 0;
}