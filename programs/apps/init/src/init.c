#include "lib.h"

int main(int argc, char** argv)
{
    // char* str1 = "hello";
    // char* str2 = "666";
    // char* sargv[256];
    // sargv[0] = str1;
    // sargv[1] = str2;
    // execve("0:/usr/bin/start.bin", sargv, 0);
    // execve("0:/usr/bin/hello.bin", 0, 0);
    execve("0:/usr/bin/shell.bin", 0, 0);
    // execve("0:/usr/bin/desktop.bin", 0, 0);
    while (1)
    {
        wait(0);
    }
    return 0;
}