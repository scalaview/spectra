#include "lib.h"
#include <stdint.h>

uint32_t current_width = 0;
uint32_t current_height = 0;
struct gui_window* shell_win;
const char* reserved_cmd = "shell# ";
int shell_max_font_count = 0;

void shell_putchar(const char ch, uint32_t reserved)
{
    screan_putchar(shell_win->buffer, ch, &current_width, &current_height, BLACK, WHITE, reserved);
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
    int32_t x = 100;
    int32_t y = 100;
    int id = 1;
    const char* title = "Shell";
    shell_win = create_gui_window(0, width, height, x, y, 0, id, title, 0);
    if (!shell_win)
    {
        printf("create window fail!\n");
        return 0;
    }
    label_struct* control_panel = create_window_control_panel(shell_win, 2);
    if (control_panel);
    shell_max_font_count = (shell_win->width - (strlen(reserved_cmd) + 1) * TEXT_FONT_STATIC_WIDTH) / TEXT_FONT_STATIC_WIDTH;
    char* buffer = (char*)malloc(sizeof(char) * shell_max_font_count);
    int buffer_size = 0;
    current_width = 0;
    current_height = GUI_CONTROL_PANEL_HEIGHT + 2;
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