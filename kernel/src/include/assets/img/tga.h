#ifndef ASSETS_IMG_TGA_H
#define ASSETS_IMG_TGA_H

#include <stdint.h>

struct tga_content {
    uint32_t width;
    uint32_t height;
    uint32_t pixels[];
};

struct tga_content* tga_parse(unsigned char* ptr, int size);

#endif


