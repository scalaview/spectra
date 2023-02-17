#include "lib.h"
#include <stdint.h>
#include "assets/img/parser.h"
#include "memory.h"

int shell_id = 100;

bool open_app_event_handler(struct gui_window* win, struct message* msg)
{
    if (msg->event == MESSAGE_MOUSE_PRESS)
    {
        if (win->id == shell_id)
        {
            execve("0:/usr/bin/shell.bin", 0, 0);
            return true;
        }
        // TODO add more app
    }
    return false;
}

static int __load_app_pixels(struct gui_window* win, uint32_t x, uint32_t y, uint32_t width, uint32_t height, draw_function draw_function)
{
    const char* filepath = "0:/data/terminal_icon128.tga";
    struct tga_content* img = 0;

    int res = img_tga_parser(filepath, (void*)(&img));
    if (res)
    {
        goto out;
    }

    draw_function(win, x, y, img->width, img->height, img->pixels);
out:
    return res;
}

static int __load_background_pixels(struct gui_window* win, uint32_t x, uint32_t y, uint32_t width, uint32_t height, draw_function draw_function)
{
    struct tga_content* background_pic = 0;
    int res = img_tga_parser("0:/data/background.tga", (void*)(&background_pic));

    if (res)
    {
        goto out;
    }

    memcpy(win->buffer->canvas, background_pic->pixels, win->buffer->pixelsize);
out:
    return res;
}

static void create_app_btns(struct gui_window* win)
{
    int32_t x = 48;
    int32_t y = 48;
    uint32_t w = 128;
    uint32_t h = 130 + TEXT_FONT_STATIC_WIDTH;

    button_struct* btn = gui_window_create_button(win, w, h, x, y, 0, shell_id, 0, &open_app_event_handler, &__load_app_pixels);
    const char* txt = "Terminal";
    gui_window_print(btn, (w - TEXT_FONT_WIDTH(txt)) / 2, h - TEXT_FONT_STATIC_WIDTH, txt, BLACK, WHITE, 0);
}

int main(int argc, char** argv)
{
    uint32_t width = 1024;
    uint32_t height = 768;
    int32_t x = 0;
    int32_t y = 0;
    int id = 2;
    const char* title = "desktop";
    struct gui_window* win = create_gui_window(0, width, height, x, y, -1, id, title, POSITION_STABLE, &__load_background_pixels);
    if (!win)
    {
        printf("create window fail!\n");
        return 0;
    }

    create_app_btns(win);

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