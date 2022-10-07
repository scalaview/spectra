#include "stdlib.h"

extern int sys_wait(int pid);

int wait(int pid)
{
    return sys_wait(pid);
}