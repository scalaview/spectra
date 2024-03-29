#include "lib.h"
#include <stdint.h>

int32_t current_x = 0;
int32_t current_y = 0;
struct gui_window* shell_win;
const char* reserved_cmd = "shell# ";
int shell_max_font_count = 0;

static int __init_shell_panel(struct gui_window* win, uint32_t x, uint32_t y, uint32_t width, uint32_t height, draw_function draw_function)
{
    draw_rect_in_absolute_position(win, x, y, width, height, WHITE);
    return 0;
}

void shell_putchar(const char ch, uint32_t reserved)
{
    screan_putchar(shell_win->buffer, ch, &current_x, &current_y, BLACK, WHITE, reserved);
}

void shell_print(const char* str, uint32_t reserved)
{
    for (char* ch = (char*)str; *ch != 0; ch++)
    {
        shell_putchar(*ch, reserved);
    }
}

static int read_cmd(char* buffer)
{
    struct message* msg = (struct message*)malloc(sizeof(struct message));
    char ch;
    int buffer_size = 0;

    while (1)
    {
        window_get_message(shell_win, msg);
        switch (msg->event)
        {
        case MESSAGE_KEY_PRESS:
            ch = (char)msg->key;
            if (ch == '\n' || buffer_size >= shell_max_font_count)
            {
                shell_putchar(ch, strlen(reserved_cmd));
                goto out;
            }
            else if (ch == '\b')
            {
                if (buffer_size > 0) {
                    buffer_size--;
                    shell_putchar(ch, strlen(reserved_cmd));
                }
            }
            else
            {
                buffer[buffer_size++] = ch;
                shell_putchar(ch, strlen(reserved_cmd));
            }
            break;
        default:
            window_consume(shell_win, msg);
            break;
        }
        if (shell_win->state == WINDOW_CLOSE) break;
    }
out:
    buffer[buffer_size] = 0;
    return buffer_size;
}

int main(int argc, char** argv)
{
    uint32_t width = 500;
    uint32_t height = 500;
    int32_t x = 150;
    int32_t y = 100;
    int id = 1;
    const char* title = "Shell";
    shell_win = create_gui_window(0, width, height, x, y, 0, id, title, 0, 0);
    if (!shell_win)
    {
        printf("create window fail!\n");
        return 0;
    }
    create_window_control_panel(shell_win, 2);
    create_window_label(shell_win, width, height - GUI_CONTROL_PANEL_HEIGHT, 0, GUI_CONTROL_PANEL_HEIGHT, 0, 3, 0, 0, WHITE, WHITE, WHITE, &__init_shell_panel);
    shell_max_font_count = (shell_win->width - (strlen(reserved_cmd) + 1) * TEXT_FONT_STATIC_WIDTH) / TEXT_FONT_STATIC_WIDTH;
    char* buffer = (char*)malloc(sizeof(char) * shell_max_font_count);
    int buffer_size = 0;
    current_x = 0;
    current_y = GUI_CONTROL_PANEL_HEIGHT + 2;
    shell_win->need_draw = true;
    while (1) {
        shell_print(reserved_cmd, 0);
        buffer_size = read_cmd(buffer);
        if (shell_win->state == WINDOW_CLOSE) break;
        if (buffer_size == 0) {
            continue;
        }
        printf("cmd: %s\n", buffer);
        // eval
    }
    gui_window_free(shell_win);

    return 0;
}