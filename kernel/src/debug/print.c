#include <stdarg.h>

#include "debug.h"
#include "printk.h"
#include "serial.h"

extern int _printk(const char* format, va_list args, serial_output_fn fn);

void _debug_putchars(const char* buffer, int size, char color)
{
    for (int i = 0; i < size; i++)
    {
        outportb(SERIAL_COM1, buffer[i]);
    }
}

void debug_printf(const char* format, ...)
{
    // #if (defined(DEBUG_LOG))
    va_list args;
    va_start(args, format);
    _printk(format, args, _debug_putchars);
    va_end(args);
    // #endif
}

