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
    if (c == BACKSPACE_ASCI) // The backspace
    {
        if (terminal_screen->column == 0 && terminal_screen->row == 0) return;

        if (terminal_screen->column == 0)
        {
            terminal_screen->row--;
            terminal_screen->column = VGA_WIDTH;
        }
        terminal_screen->column--;
        screen_put_char(terminal_screen, 0, 0);

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
    char text[STR_BUF_SIZE];
    char* c = itoa(digits, text);
    int size = 0;

    for (; *c; c++)
    {
        buffer[position++] = *c;
        size++;
    }
    return size;
}

static int __read_string(char* buffer, int position, int* total, const char* str, serial_output_fn fn)
{
    size_t len = strlen(str);
    for (int i = 0; i < len;i++)
    {
        buffer[position++] = str[i];
        if (str[i] == '\n' || position >= BUFFER_SIZE)
        {
            // Flush to screen
            fn(buffer, position, 0xf);
            *total += position;
            position = 0;
        }

    }
    *total += position;
    return position;
}

static int __hex_to_string(char* buffer, int position, int d, int sg, int64_t i, int letbase, serial_output_fn fn)
{
    char text[STR_BUF_SIZE];
    char* ptr;
    memset(text, 0, sizeof(text));
    char negative = 0;
    int t, total = 0;
    uint64_t u = i;

    if (sg && d == 10 && i < 0)
    {
        negative = 1;
        u = -i;
    }
    ptr = text + STR_BUF_SIZE - 1;

    while (u)
    {
        t = u % d;
        if (t >= 10) t += letbase - '0' - 10;
        *--ptr = t + '0';
        u /= d;
    }

    if (negative) *--ptr = '-';

    __read_string(buffer, position, &total, ptr, fn);
    return total;
}

void terminal_screen_initialize()
{
    terminal_screen.buffer = (uint16_t*)phy2vir(VGA_ADDRESS);
    memset(terminal_screen.buffer, 0, 2 * VGA_WIDTH * VGA_HEIGHT);
    terminal_screen.column = 0;
    terminal_screen.row = 0;
}

int _printk(const char* format, va_list args, serial_output_fn fn)
{
    char buffer[BUFFER_SIZE];
    int buffer_size = 0, total = 0;
    int64_t interger = 0;
    char* string = 0;
    const char* p = 0;
    memset(buffer, 0, sizeof(buffer));

    for (p = format; *p; p++)
    {
        if (*p != '%')
        {
            flush_buffer(fn, buffer, buffer_size, 0, 0xf);
            buffer[buffer_size++] = *p;
            continue;
        }
        switch (*++p)
        {
        case 'd':
            interger = va_arg(args, int32_t);
            flush_buffer(fn, buffer, buffer_size, STR_BUF_SIZE, 0xf);
            buffer_size += __hex_to_string(buffer, buffer_size, 10, 1, interger, '0', fn);
            break;
        case 'u':
            interger = va_arg(args, int64_t);
            flush_buffer(fn, buffer, buffer_size, STR_BUF_SIZE, 0xf);
            buffer_size += __hex_to_string(buffer, buffer_size, 10, 0, interger, '0', fn);
            break;
        case 'x':
            interger = va_arg(args, int64_t);
            flush_buffer(fn, buffer, buffer_size, STR_BUF_SIZE, 0xf);
            buffer_size += __hex_to_string(buffer, buffer_size, 16, 0, interger, 'a', fn);
            break;
        case 'X':
            interger = va_arg(args, int64_t);
            flush_buffer(fn, buffer, buffer_size, STR_BUF_SIZE, 0xf);
            buffer_size += __hex_to_string(buffer, buffer_size, 16, 0, interger, 'A', fn);
            break;
        case 's':
            string = va_arg(args, char*);
            buffer_size = __read_string(buffer, buffer_size, &total, string, fn);
            break;
        default:
            buffer[buffer_size++] = '%';
            p--;
            break;
        }
    }
    fn(buffer, buffer_size, 0xf);
    total += buffer_size;
    return total;
}

int printk(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int buffer_size = _printk(format, args, print_to_screen);
    va_end(args);
    return buffer_size;
}
