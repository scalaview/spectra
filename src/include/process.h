#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

struct process
{
    uint16_t id;
    struct task* primary;

};

#endif