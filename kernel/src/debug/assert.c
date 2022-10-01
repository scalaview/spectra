#include "assert.h"
#include "printk.h"

extern void halt();

void assert_error(const char* file, const uint64_t line)
{
    printk("\nASSERT ERROR: %s:%d", file, line);
    halt();
}
