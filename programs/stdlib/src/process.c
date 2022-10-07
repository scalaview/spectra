#include "stdlib.h"

extern int sys_wait(int pid);

int wait(int pid)
{
    return sys_wait(pid);
}

extern int sys_execve(const char* pathname, const char* argv, const char* envp, int ring_lev);

int execve(const char* pathname, const char* argv, const char* envp)
{
    return sys_execve(pathname, argv, envp, RING3);
}
