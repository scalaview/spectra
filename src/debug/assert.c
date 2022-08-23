#include "assert.h"
#include "printk.h"

void assert_error(const char* file, const uint64_t line)
{
    printk("\nASSERT ERROR: %s:%d", file, line);
    while (1) {}
}
