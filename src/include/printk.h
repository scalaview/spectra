#ifndef PRINTK_H
#define PRINTK_H

#define VGA_WIDTH 160
#define VGA_HEIGHT 24
#define VGA_ADDRESS  0xb8000
#define BACKSPCE_ASCI  0x0008

#include <stdint.h>

struct terminal_screen {
    uint16_t* buffer;
    uint16_t row;
    uint16_t column;
};

// https://en.wikipedia.org/wiki/Printk
int printk(const char* format, ...);

#endif
