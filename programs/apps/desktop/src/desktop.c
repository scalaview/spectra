#include "lib.h"
#include <stdint.h>

uint64_t current_width = 0;
uint64_t current_height = 0;

void window_putchar(struct gui_window* win, const char ch)
{
    struct screen_buffer* buffer = win->buffer;
    if (ch == '\n' || current_width == buffer->width)
    {
        current_height = current_height + 8;
        current_width = 0;
        return;
    }
    if (ch == '\b')
    {
        char c = ' ';
        if (current_width >= 8)
        {
            current_width = current_width - 8;
            gfx_puts(current_width, current_height, 0xff000000, 0xffffffff, &c, buffer);
        }
        return;
    }
    if (current_width + 8 > buffer->width)
    {
        current_height = current_height + 8;
        current_width = 0;
    }
    gfx_puts(current_width, current_height, 0xff000000, 0xffffffff, &ch, buffer);
    current_width = current_width + 8;
}

int main(int argc, char** argv)
{
    uint32_t width = 500;
    uint32_t height = 500;
    struct gui_window* gui_win = create_window_content(100, 100, width, height, 0xffffffff);
    struct screen_buffer* buffer = gui_win->buffer;
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
    current_width = 0;
    current_height = 21;
    gui_win->need_draw = true;
    struct message* msg = (struct message*)malloc(sizeof(struct message));
    while (1)
    {
        window_get_message(gui_win, msg);
        switch (msg->event)
        {
        case MESSAGE_KEY_PRESS:
            window_putchar(gui_win, msg->key);
            break;

        default:
            break;
        }

    }
    return 0;
}