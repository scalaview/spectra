#include "unistd.h"

extern int sys_sleep(int seconds);

int sleep(int seconds)
{
    return sys_sleep(seconds);
}
