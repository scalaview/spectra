#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>

#define MESSAGE_MOUSE_MOVE          0x1
#define MESSAGE_MOUSE_PRESS 	    0x2
#define MESSAGE_MOUSE_RELEASE	    0x3
#define MESSAGE_MOUSE_DRAG	        0x4
#define MESSAGE_CLOSE               0x5

#define MESSAGE_KEY_PRESS		    0x5
#define MESSAGE_KEY_RELEASE         0x6

struct message
{
    uint16_t 		event;
    int32_t 		key;

    uint16_t 		handle;
    int16_t 		x;
    int16_t 		y;
    int16_t 		diff_x;
    int16_t 		diff_y;
};

#endif