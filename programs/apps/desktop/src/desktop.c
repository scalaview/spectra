#include "lib.h"
#include <stdint.h>
#include "assets/img/parser.h"
#include "memory.h"

int main(int argc, char** argv)
{
    struct tga_content* background_pic = 0;
    int res = img_tga_parser("0:/data/background.tga", (void*)(&background_pic));
    if (res);
    uint32_t width = 1024;
    uint32_t height = 768;
    int32_t x = 0;
    int32_t y = 0;
    int id = 2;
    const char* title = "desktop";
    struct gui_window* win = create_gui_window(0, width, height, x, y, -1, id, title, POSITION_STABLE);
    if (!win)
    {
        printf("create window fail!\n");
        return 0;
    }
    memcpy(win->buffer->canvas, background_pic->pixels, win->buffer->pixelsize);
    win->need_draw = true;
    struct message* msg = (struct message*)malloc(sizeof(struct message));
    while (1)
    {
        window_get_message(win, msg);
        if (msg->event)
        {
            window_consume(win, msg);
            if (win->state == WINDOW_CLOSE) break;
            win->need_draw = true;
        }
    }
    gui_window_free(win);
    return 0;
}