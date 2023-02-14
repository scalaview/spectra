#include "gui/gui.h"
#include "stdlib.h"
#include "memory.h"
#include "assets/color.h"
#include "string.h"
#include "stdio.h"
#include "driver/mouse/mouse.h"
#include "gui/button.h"

struct gui_window* create_gui_window(struct gui_window* parent, uint32_t width, uint32_t height, int32_t x, int32_t y, int id, const char* title);

void window_add_container(struct gui_window* parent, struct gui_window* new_win)
{
    struct gui_window* current = parent;
    while (current->next) current = current->next;
    current->next = new_win;
}

static struct gui_window* __gui_windoww_find_child(struct gui_window* win, int16_t x, int16_t y)
{
    struct gui_window* current = win->next;
    while (current)
    {
        printf("inside gui_windoww_find_child\n");
        if (x >= current->x && x <= current->x + current->width && y >= current->y && y <= current->y + current->height) return current;
        current = current->next;
    }
    return 0;
}

struct gui_window* gui_window_get_root(struct gui_window* win)
{
    struct gui_window* current = win;
    while (current)
    {
        if (!current->parent) break;
        current = current->parent;
    }
    return current;
}

static struct gui_window* __gui_window_get_tail(struct gui_window* win)
{
    struct gui_window* current = win;
    while (current)
    {
        if (!current->next) break;
        current = current->next;
    }
    return current;
}


static bool __gui_window_default_procedure(struct gui_window* win, struct message* msg)
{
    switch (msg->event)
    {
    case MESSAGE_MOUSE_PRESS:
        printf("MESSAGE_MOUSE_PRESS");
        if (msg->key & MOUSE_LEFT_DRAG)
        {
            struct gui_window* target = 0;
            if (win->dragged)  target = win->dragged;
            else    target = __gui_windoww_find_child(win, msg->x, msg->y);
            if (target)
            {
                msg->event = MESSAGE_MOUSE_DRAG;
                win->dragged = target;
                return window_consume_message(target, msg);
            }
        }
        break;
    case MESSAGE_MOUSE_RELEASE:
        printf("MESSAGE_MOUSE_RELEASE");
        break;

    case MESSAGE_MOUSE_DRAG:
        printf("MESSAGE_MOUSE_RELEASE");
        struct gui_window* root_win = gui_window_get_root(win);
        printf("MESSAGE_MOUSE_DRAG before x: %d, y: %d, diff_x: %d, diff_y:%d\n", root_win->x, root_win->y, msg->diff_x, msg->diff_y);
        root_win->x += msg->diff_x;
        root_win->y += msg->diff_y;
        printf("MESSAGE_MOUSE_DRAG after x: %d, y: %d, diff_x: %d, diff_y:%d\n", root_win->x, root_win->y, msg->diff_x, msg->diff_y);
        return true;
    case MESSAGE_CLOSE:
        printf("=================close========================\n");
        break;

    }
    win->dragged = 0;
    return false;
}

void get_absolute_position(struct gui_window* parent, int32_t x, int32_t y, int32_t* x_out, int32_t* y_out)
{
    struct gui_window* current = parent;
    int32_t abs_x = x;
    int32_t abs_y = y;
    while (current)
    {
        if (current->parent)
        {
            abs_x = abs_x + current->x;
            abs_y = abs_y + current->y;
        }
        current = current->parent;
    }
    *x_out = abs_x;
    *y_out = abs_y;
}

void draw_rect_in_absolute_position(struct gui_window* parent, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color)
{
    int32_t abs_x = 0;
    int32_t abs_y = 0;
    get_absolute_position(parent, x, y, &abs_x, &abs_y);
    draw_rect(abs_x, abs_y, width, height, color, parent->buffer);
}

label_struct* create_window_lable(struct gui_window* parent, uint32_t width, uint32_t height, int32_t x, int32_t y, int id, const char* title, uint32_t color, uint32_t t_color, uint32_t tb_color)
{
    label_struct* label = create_gui_window(parent, width, height, x, y, id, title);
    if (!label) return 0;
    struct screen_buffer* buffer = label->buffer;
    draw_rect_in_absolute_position(parent, x, y, width, height, color);
    gfx_puts((width - TEXT_FONT_WIDTH(title)) / 2, (height - TEXT_FONT_HEIGHT(title)) / 2, t_color, tb_color, title, buffer);
    return label;
}

button_struct* gui_window_create_close_button(struct gui_window* win, int id)
{
    int32_t x = win->width - GUI_CONTROL_PANEL_CLOSE_BTN_WIDTH - TEXT_FONT_STATIC_WIDTH;
    int32_t y = (win->height - GUI_CONTROL_PANEL_CLOSE_BTN_HEIGHT) / 2;
    button_struct* btn = gui_window_create_button(win, GUI_CONTROL_PANEL_CLOSE_BTN_WIDTH, GUI_CONTROL_PANEL_CLOSE_BTN_HEIGHT, x, y, id, 0);
    if (!btn) return 0;
    // TODO replace with close img
    draw_rect_in_absolute_position(win, x, y, GUI_CONTROL_PANEL_CLOSE_BTN_WIDTH, GUI_CONTROL_PANEL_CLOSE_BTN_HEIGHT, RED);
    return btn;
}

label_struct* create_window_control_panel(struct gui_window* win, int id)
{
    label_struct* panel = create_window_lable(win, win->width, GUI_CONTROL_PANEL_HEIGHT, 0, 0, id, win->title, BLACK, WHITE, BLACK);
    if (!panel) return 0;

    button_struct* close_btn = gui_window_create_close_button(panel, GUI_CONTROL_PANEL_CLOSS_BUTTON_ID);
    close_btn->custom_procedure = 0;
    return panel;
}

struct gui_window* create_gui_window(struct gui_window* parent, uint32_t width, uint32_t height, int32_t x, int32_t y, int id, const char* title)
{
    struct gui_window* new_win;
    if (!parent)
    {
        // create screen buffer
        new_win = create_window_content(x, y, width, height, 0xffffffff);
        if (!new_win->buffer) return 0;
    }
    else
    {
        new_win = (struct gui_window*)malloc(sizeof(struct gui_window));
        if (!new_win) return 0;
        new_win->buffer = parent->buffer;
        new_win->parent = parent;
        new_win->handle = parent->handle;
        window_add_container(parent, new_win);
    }
    if (title)
    {
        new_win->title = (char*)malloc(strlen(title));
        if (!new_win->title)
        {
            free(new_win);
            return 0;
        }
        memcpy(new_win->title, (void*)title, strlen(title));
    }
    new_win->id = id;
    new_win->height = height;
    new_win->width = width;
    new_win->x = x;
    new_win->y = y;
    new_win->need_draw = false;
    new_win->default_procedure = &__gui_window_default_procedure;

    return new_win;
}

void screan_putchar(struct screen_buffer* buffer, const char cha, uint32_t* current_width, uint32_t* current_height, uint32_t t_color, uint32_t tb_color, uint32_t reserved)
{
    char ch[2] = { cha , 0 };

    if (ch[0] == '\n')
    {
        *current_height = *current_height + TEXT_FONT_STATIC_WIDTH;
        *current_width = 0;
        return;
    }
    if (ch[0] == '\b')
    {
        char c = ' ';
        if (*current_width >= TEXT_FONT_STATIC_WIDTH + reserved)
        {
            *current_width = *current_width - TEXT_FONT_STATIC_WIDTH;
            gfx_puts(*current_width, *current_height, t_color, tb_color, &c, buffer);
        }

        return;
    }
    if (*current_width + TEXT_FONT_STATIC_WIDTH > buffer->width)
    {
        *current_height = *current_height + TEXT_FONT_STATIC_WIDTH;
        *current_width = 0;
    }
    gfx_puts(*current_width, *current_height, t_color, tb_color, ch, buffer);
    *current_width = *current_width + TEXT_FONT_WIDTH(ch);
}

bool window_consume_message(struct gui_window* win, struct message* msg)
{
    bool consumed = false;
    if (win->custom_procedure != NULL) {
        consumed = win->custom_procedure(win, msg);
    }
    if (!consumed) {
        consumed = win->default_procedure(win, msg);
    }
    return consumed;
}

bool window_consume_message_simple(struct gui_window* win, uint16_t event)
{
    struct message msg;
    msg.event = event;
    return window_consume_message(win, &msg);
}

void window_consume(struct gui_window* win, struct message* msg)
{
    for (struct gui_window* w = win; w != NULL; w = w->next) {
        if (window_consume_message(w, msg)) {
            break;
        }
    }
}

void gui_window_free(struct gui_window* win)
{
    int id = win->handle;
    win->need_draw = false;
    struct gui_window* current = win->next;
    while (current)
    {
        free(current->title);
        free(current);
        current = current->next;
    }
    free(win->title);
    free(win);
    sys_window_free(id);
}