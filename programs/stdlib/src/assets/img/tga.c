#include "assets/img/tga.h"

#define USER_LAND
#include "assets/img/std_tga.h"


struct tga_content* tga_parse(unsigned char* ptr, int size)
{
    return (struct tga_content*)std_tga_parse(ptr, size);
}