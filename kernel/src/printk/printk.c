#include <stdarg.h>
#include <stddef.h>

#include "string.h"
#include "printk.h"
#include "kmemory.h"
#include "assert.h"
#include "config.h"

static struct terminal_screen terminal_screen;

uint16_t make_char(char c, char color)
{
    return (color << 8) | c;
}

void screen_put_char(struct terminal_screen* terminal_screen, char c, char color)
{
    terminal_screen->buffer[terminal_screen->row * VGA_WIDTH + terminal_screen->column] = make_char(c, color);
}

static void increase_row(struct terminal_screen* terminal_screen)
{
    terminal_screen->row++;
    if (terminal_screen->row >= VGA_HEIGHT)
    {
        memcpy(terminal_screen->buffer, terminal_screen->buffer + VGA_WIDTH, 2 * VGA_WIDTH * (VGA_HEIGHT - 1));
        memset(terminal_screen->buffer + VGA_WIDTH * (VGA_HEIGHT - 1), 0, 2 * VGA_WIDTH);
        terminal_screen->row--;
    }
}

void screen_write_char(struct terminal_screen* terminal_screen, char c, char color)
{
    if (c == '\n')
    {
        terminal_screen->column = 0;
        increase_row(terminal_screen);
        return;
    }
    if (c == BACKSPCE_ASCI) // The backspace
    {
        // TODO
        return;
    }
    screen_put_char(terminal_screen, c, color);
    terminal_screen->column++;
    if (terminal_screen->column >= VGA_WIDTH)
    {
        terminal_screen->column = 0;
        increase_row(terminal_screen);
    }
}

void print_to_screen(const char* buffer, int size, char color)
{
    for (int i = 0; i < size; i++)
    {
        screen_write_char(&terminal_screen, buffer[i], color);
    }
}

int decimal_to_string(char* buffer, int position, int64_t digits)
{
    char text[20];
    char* c = itoa(digits, text);
    int size = 0;

    for (; *c; c++)
    {
        buffer[position++] = *c;
        size++;
    }
    return size;
}

int hex_to_string(char* buffer, int position, int64_t i)
{
    static char hex[16] = "0123456789ABCDEF";
    char text[21];
    memset(text, 0, sizeof(text));
    char negative = 1;
    int size = 0;

    if (i >= 0)
    {
        negative = 0;
    }
    text[size++] = 'H';

    do {
        text[size++] = hex[i % 16];
        i /= 16;
    } while (i != 0);

    if (negative)
        text[size++] = '-';

    for (int i = size - 1; i >= 0; i--) {
        buffer[position++] = text[i];
    }
    return size;
}

int read_string(char* buffer, int position, const char* str)
{
    size_t len = strlen(str);
    for (int i = 0; i < len;i++)
    {
        buffer[position++] = str[i];
        if (str[i] == '\n' || position >= BUFFER_SIZE)
        {
            // Flush to screen
            print_to_screen(buffer, position, 0xf);
            position = 0;
        }

    }
    return position;
}

void terminal_screen_initialize()
{
    terminal_screen.buffer = (uint16_t*)phy2vir(VGA_ADDRESS);
    memset(terminal_screen.buffer, 0, 2 * VGA_WIDTH * VGA_HEIGHT);
    terminal_screen.column = 0;
    terminal_screen.row = 0;
}

int printk(const char* format, ...)
{
    va_list args;
    char buffer[1024];
    int buffer_size = 0;
    int64_t interger = 0;
    char* string = 0;
    const char* p = 0;
    va_start(args, format);
    memset(buffer, 0, sizeof(buffer));

    for (p = format; *p; p++)
    {
        if (*p != '%')
        {
            buffer[buffer_size++] = *p;
            assert(buffer_size < 1024);
            continue;
        }
        switch (*++p)
        {
        case 'd':
            interger = va_arg(args, int64_t);
            buffer_size += decimal_to_string(buffer, buffer_size, interger);
            break;
        case 'x':
            interger = va_arg(args, int64_t);
            buffer_size += hex_to_string(buffer, buffer_size, interger);
            break;
        case 's':
            string = va_arg(args, char*);
            buffer_size = read_string(buffer, buffer_size, string);
            break;
        default:
            buffer[buffer_size++] = '%';
            p--;
            break;
        }
        assert(buffer_size < 1024);
    }
    print_to_screen(buffer, buffer_size, 0xf);
    va_end(args);
    return buffer_size;
}