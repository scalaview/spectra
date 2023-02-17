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
#define MOUSE_STATUS                0x64
#define MOUSE_F_BIT                 0x20
#define MOUSE_BBIT                  0x01

#define MOUSE_LEFT_CLICK   		0b00001
#define MOUSE_RIGHT_CLICK  		0b00010
#define MOUSE_MIDDLE_CLICK 		0b00100
#define MOUSE_LEFT_DRAG  		0b01000

extern int16_t mouse_x;
extern int16_t mouse_y;

void mouse_initialize();

#endif