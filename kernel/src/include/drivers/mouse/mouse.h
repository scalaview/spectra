#ifndef MOUSE_H
#define MOUSE_H

#include <stdint.h>

#define PS2_PORT_0X64               0x64
#define PS2_PORT_0X60               0x60
#define MOUSE_NOTREADY              (1<<1)
#define MOUSE_NODATA                1
#define ISR_MOUSE_INTERRUPT         0x2c
#define SEND_0XD4_BYTE              0xd4
#define ENABLE_AUXILIARY_DEVICE     0xA8
#define MOUSE_SET_DEFUAL            0xF6
#define MOUSE_ENABLE                0xF4
#define MOUSE_AVALIABLE             0x08

extern int16_t mouse_x;
extern int16_t mouse_y;

void mouse_initialize();

#endif