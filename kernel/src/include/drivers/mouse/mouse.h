#ifndef MOUSE_H
#define MOUSE_H

#include <stdint.h>

#define PS2_PORT_0X64               0x64
#define PS2_PORT_0X60               0x60
#define MOUSE_NOTREADY              (1<<1)
#define MOUSE_NODATA                1
#define ISR_MOUSE_INTERRUPT         0x2c

void mouse_initialize();

#endif