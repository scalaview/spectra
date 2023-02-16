#include "lib.h"
#include <stdint.h>
#include "assets/img/parser.h"
#include "memory.h"

bool open_app_event_handler(struct gui_window* win, struct message* msg)
{
    printf("====================app click 1======================\n");
    // execve("0:/usr/bin/shell.bin", 0, 0);
    printf("====================app click 2======================\n");

    return true;
}


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

    // button_struct* gui_window_create_button(struct gui_window* parent, uint32_t width, uint32_t height, int32_t x, int32_t y, int32_t z, int id, uint16_t attributes, window_procedure procedure)
    button_struct* btn = gui_window_create_button(win, 80, 80, 48, 48, 0, 2, 0, &open_app_event_handler);
    draw_rect_in_absolute_position(win, 48, 48, 80, 80, WHITE);

    gui_window_print(btn, 0, 80 - TEXT_FONT_STATIC_WIDTH - 2, "test", BLACK, WHITE, 0);

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