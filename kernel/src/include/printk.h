#ifndef PRINTK_H
#define PRINTK_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_ADDRESS  0xb8000
#define BACKSPACE_ASCI  0x0008
#define STR_BUF_SIZE    21
#define flush_buffer(fn, buffer, buffer_size, pre_buf_size, col) do { \
    if(buffer_size + pre_buf_size >= BUFFER_SIZE) \
    {\
        fn(buffer, buffer_size, col);\
        total += buffer_size;\
        buffer_size = 0;\
    }\
} while(0)
#include <stdint.h>

#define BUFFER_SIZE     1024

struct terminal_screen {
    uint16_t* buffer;
    uint16_t row;
    uint16_t column;
}__attribute__((packed));

typedef void (*serial_output_fn)(const char* buffer, int size, char color);

// https://en.wikipedia.org/wiki/Printk
int printk(const char* format, ...);
void terminal_screen_initialize();
void print_to_screen(const char* buffer, int size, char color);

#endif
