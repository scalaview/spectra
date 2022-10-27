#include "assert.h"
#include "debug.h"

extern void halt();

void assert_error(const char* file, const uint64_t line)
{
    debug_printf("\nASSERT ERROR: %s:%d", file, line);
    halt();
}
