#include "lib.h"
#include <stdint.h>

int main(int argc, char** argv)
{
    uint32_t width = 500;
    uint32_t height = 500;
    struct screen_buffer* buffer = create_window_content(100, 100, width, height, 0xffffffff);
    if (!buffer)
    {
        printf("create window fail!\n");
        return 0;
    }

    draw_rect(0, 0, width, 20, 0xff000000, buffer);
    char* c = "hello word!";
    printf("x: %d\n", (width - TEXT_FONT_WIDTH(c)) / 2);
    printf("y: %d\n", (20 - TEXT_FONT_HEIGHT(c)) / 2);
    gfx_puts((width - TEXT_FONT_WIDTH(c)) / 2, (20 - TEXT_FONT_HEIGHT(c)) / 2, 0xffffffff, 0x0, c, buffer);

    return 0;
}