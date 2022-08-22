#include <stdarg.h>
#include "string.h"
#include "printk.h"

static struct terminal_screen terminal_screen = { (uint16_t*)VGA_ADDRESS, 0, 0 };

uint16_t make_char(char c, char color)
{
    return (color << 8) | c;
}

void screen_put_char(struct terminal_screen* terminal_screen, char c, char color)
{
    terminal_screen->buffer[terminal_screen->row * VGA_WIDTH + terminal_screen->column] = make_char(c, color);

}

void screen_write_char(struct terminal_screen* terminal_screen, char c, char color)
{
    if (c == '\n')
    {
        terminal_screen->column = 0;
        terminal_screen->row++;
        return;;;;
    }
    if (c == BACKSPCE_ASCI) // The backspace
    {
        return;
    }
    screen_put_char(terminal_screen, c, color);
    terminal_screen->column++;
    if (terminal_screen->column >= VGA_WIDTH)
    {
        terminal_screen->column = 0;
        terminal_screen->row++;
    }
}

void write_screen(const char* buffer, int size, struct terminal_screen* terminal_screen, char color)
{
    for (int i = 0; i < size; i++)
    {
        screen_write_char(terminal_screen, buffer[i], color);
    }
}

int decimal_to_string(char* buffer, int position, int64_t digits)
{
    char text[20];
    itoa(digits, text);
    int size = 0;

    for (char* c = text; *c; c++)
    {
        buffer[position++] = *text;
        size++;
    }
    return size;
}

int printk(const char* format, ...)
{
    va_list args;
    char buffer[1024];
    int buffer_size = 0;
    int64_t interger = 0;
    // char* string = 0;
    const char* p = 0;
    va_start(args, format);

    for (p = format; *p; p++)
    {
        if (*p != '%')
        {
            buffer[buffer_size++] = *p;
            continue;
        }
        switch (*++p)
        {
        case 'd':
            interger = va_arg(args, int64_t);
            buffer_size += decimal_to_string(buffer, buffer_size, interger);
            break;
        default:
            break;
        }
    }
    write_screen(buffer, buffer_size, &terminal_screen, 0xf);
    va_end(args);
    return buffer_size;
}