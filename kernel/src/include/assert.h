#ifndef DEBUG_ASSERT_H
#define DEBUG_ASSERT_H
#include <stdint.h>

#define assert(e) do {\
    if(!(e))\
        assert_error(__FILE__, __LINE__);\
} while (0)

void assert_error(const char* file, const uint64_t line);

#endif