#include "lib.h"
#include <stdint.h>

int main(int argc, char** argv)
{
    uint32_t width = 600;
    uint32_t height = 600;
    int64_t x = 200;
    int64_t y = 200;
    int id = 2;
    const char* title = "untitle";
    struct gui_window* win = create_gui_window(0, width, height, x, y, id, title);
    if (!win)
    {
        printf("create window fail!\n");
        return 0;
    }
    create_window_control_panel(win, 2);
    win->need_draw = true;
    struct message* msg = (struct message*)malloc(sizeof(struct message));
    while (1)
    {
        window_get_message(win, msg);
        if (msg->event)
        {
            window_consume(win, msg);
            win->need_draw = true;
        }
    }

    return 0;
}