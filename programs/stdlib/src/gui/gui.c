#include "gui/gui.h"
#include "stdlib.h"
#include "memory.h"
#include "assets/color.h"
#include "string.h"
#include "stdio.h"
#include "driver/mouse/mouse.h"
#include "gui/button.h"
#include "gui/label.h"
#include "assets/img/tga.h"
#include "assets/img/parser.h"

struct gui_window* create_gui_window(struct gui_window* parent, uint32_t width, uint32_t height, int32_t x, int32_t y, int32_t z, int id, const char* title, uint16_t attributes, pixels_producteur producteur);
bool window_consume(struct gui_window* win, struct message* msg);

static void __window_add_child(struct gui_window* parent, struct gui_window* new_win)
{
    struct gui_window* child = parent->children;
    if (!child)
    {
        parent->children = new_win;
        return;
    }
    while (child->other) child = child->other;
    child->other = new_win;

}

static bool __gui_window_pointer_inside(struct gui_window* win, int16_t x, int16_t y)
{
    printf("pointer_inside? %d, x:%d, y:%d, w.x:%d, w.y%d, w.w:%d, w.h%d\n", win->id, x, y, win->x, win->y, win->width, win->height);
    return (x >= win->x && x <= win->x + win->width && y >= win->y && y <= win->y + win->height);
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

static bool __gui_window_control_panel_default_procedure(struct gui_window* win, struct message* msg)
{

    if (msg->event == MESSAGE_MOUSE_PRESS)
    {
        msg->event = MESSAGE_MOUSE_DRAG;
        return true;
    }
    return false;
}

static bool __gui_window_default_procedure(struct gui_window* win, struct message* msg)
{
    switch (msg->event)
    {
    case MESSAGE_MOUSE_PRESS:
        printf("MESSAGE_MOUSE_PRESS");
        break;
    case MESSAGE_MOUSE_RELEASE:
        printf("MESSAGE_MOUSE_RELEASE");
        break;
    case MESSAGE_MOUSE_DRAG:
        // handle_drag:
        printf("MESSAGE_MOUSE_RELEASE");
        struct gui_window* root_win = gui_window_get_root(win);
        printf("MESSAGE_MOUSE_DRAG before x: %d, y: %d, diff_x: %d, diff_y:%d\n", root_win->x, root_win->y, msg->diff_x, msg->diff_y);
        root_win->x += msg->diff_x;
        root_win->y += msg->diff_y;
        printf("MESSAGE_MOUSE_DRAG after x: %d, y: %d, diff_x: %d, diff_y:%d\n", root_win->x, root_win->y, msg->diff_x, msg->diff_y);
        root_win->dragged = win;
        return true;
    case MESSAGE_CLOSE:
        win->state = WINDOW_CLOSE;
        return true;
    }
    win->dragged = 0;
    return false;
}

void get_absolute_position(struct gui_window* win, int32_t x, int32_t y, int32_t* x_out, int32_t* y_out)
{
    struct gui_window* current = win;
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

bool __close_button_default_procedure(struct gui_window* win, struct message* msg)
{
    if (msg->event == MESSAGE_MOUSE_PRESS)
    {
        msg->event = MESSAGE_CLOSE;
        return true;
    }
    return false;
}

void draw_with_pixels(struct gui_window* win, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t pixels[])
{
    int32_t abs_x = 0;
    int32_t abs_y = 0;
    get_absolute_position(win, x, y, &abs_x, &abs_y);
    draw_transparent_icon(abs_x, abs_y, width, height, pixels, win->buffer);
}

int draw_with_file(struct gui_window* win, uint32_t x, uint32_t y, uint32_t width, uint32_t height, const char* filepath)
{
    struct tga_content* img = 0;

    int res = img_tga_parser(filepath, (void*)(&img));
    if (res)
    {
        goto out;
    }

    draw_with_pixels(win, x, y, width, height, img->pixels);

out:
    return res;
}

static int __load_close_btn_pixels(struct gui_window* win, uint32_t x, uint32_t y, uint32_t width, uint32_t height, draw_function draw_function)
{
    const char* filepath = "0:/data/close24.tga";
    struct tga_content* img = 0;

    int res = img_tga_parser(filepath, (void*)(&img));
    if (res)
    {
        goto out;
    }

    draw_function(win, x, y, width, height, img->pixels);
out:
    return res;
}

button_struct* gui_window_create_close_button(struct gui_window* win, int id, pixels_producteur producteur)
{
    int32_t x = win->width - GUI_CONTROL_PANEL_CLOSE_BTN_WIDTH - TEXT_FONT_STATIC_WIDTH;
    int32_t y = (win->height - GUI_CONTROL_PANEL_CLOSE_BTN_HEIGHT) / 2;
    button_struct* btn = gui_window_create_button(win, GUI_CONTROL_PANEL_CLOSE_BTN_WIDTH, GUI_CONTROL_PANEL_CLOSE_BTN_HEIGHT, x, y, 0, id, POSITION_STABLE, 0, producteur);
    if (!btn) return 0;
    // TODO replace with close img
    btn->default_procedure = &__close_button_default_procedure;
    return btn;
}

label_struct* create_window_control_panel(struct gui_window* win, int id)
{
    label_struct* panel = create_window_label(win, win->width, GUI_CONTROL_PANEL_HEIGHT, 0, 0, 0, id, win->title, POSITION_STABLE, BLACK, WHITE, BLACK, 0);
    if (!panel) return 0;
    panel->default_procedure = &__gui_window_control_panel_default_procedure;
    gui_window_create_close_button(panel, GUI_CONTROL_PANEL_CLOSS_BUTTON_ID, &__load_close_btn_pixels);
    return panel;
}

struct gui_window* create_gui_window(struct gui_window* parent, uint32_t width, uint32_t height, int32_t x, int32_t y, int32_t z, int id, const char* title, uint16_t attributes, pixels_producteur producteur)
{
    struct gui_window* new_win;
    if (!parent)
    {
        // create screen buffer
        new_win = create_window_content(x, y, z, width, height, 0xffffffff, attributes);
        if (!new_win->buffer) return 0;
    }
    else
    {
        new_win = (struct gui_window*)malloc(sizeof(struct gui_window));
        if (!new_win) return 0;
        new_win->buffer = parent->buffer;
        new_win->parent = parent;
        new_win->handle = parent->handle;
        new_win->x = x;
        new_win->y = y;
        new_win->z = z;
        __window_add_child(parent, new_win);
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
    new_win->need_draw = false;
    new_win->default_procedure = &__gui_window_default_procedure;
    if (producteur)
    {
        if (new_win->parent) producteur(new_win->parent, x, y, width, height, &draw_with_pixels);
        else  producteur(new_win, x, y, width, height, &draw_with_pixels);
    }
    return new_win;
}

void screan_putchar(struct screen_buffer* buffer, const char cha, int32_t* current_x, int32_t* current_y, uint32_t t_color, uint32_t tb_color, uint32_t reserved)
{
    char ch[2] = { cha , 0 };

    if (ch[0] == '\n')
    {
        *current_y = *current_y + TEXT_FONT_STATIC_WIDTH;
        *current_x = 0;
        return;
    }
    if (ch[0] == '\b')
    {
        char c = ' ';
        if (*current_x >= TEXT_FONT_STATIC_WIDTH + reserved)
        {
            *current_x = *current_x - TEXT_FONT_STATIC_WIDTH;
            gfx_puts(*current_x, *current_y, t_color, tb_color, &c, buffer);
        }

        return;
    }
    if (*current_x + TEXT_FONT_STATIC_WIDTH > buffer->width)
    {
        *current_y = *current_y + TEXT_FONT_STATIC_WIDTH;
        *current_x = 0;
    }
    gfx_puts(*current_x, *current_y, t_color, tb_color, ch, buffer);
    *current_x = *current_x + TEXT_FONT_WIDTH(ch);
}

void gui_window_print(struct gui_window* win, int32_t x, int32_t y, const char* str, uint32_t t_color, uint32_t tb_color, uint32_t reserved)
{
    int32_t current_x = x;
    int32_t current_y = y;
    get_absolute_position(win, x, y, &current_x, &current_y);

    while (*str != 0)
    {
        screan_putchar(win->buffer, *str++, &current_x, &current_y, t_color, tb_color, reserved);
    }
}

bool window_consume_message(struct gui_window* win, struct message* msg)
{
    printf("window_consume_message start %d\n", win->id);
    if (!__gui_window_pointer_inside(win, msg->x, msg->y))
    {
        printf("no inside window\n");
        return false;
    }
    bool consumed = false;
    if (win->custom_procedure != NULL) {
        // printf("custom_procedure start %d, %d\n", win->id, msg->event);
        consumed = win->custom_procedure(win, msg);
        // printf("custom_procedure end %d, %d\n", win->id, msg->event);
    }
    if (!consumed) {
        // printf("default_procedure start %d, %d\n", win->id, msg->event);
        consumed = win->default_procedure(win, msg);
        // printf("default_procedure end %d, %d\n", win->id, msg->event);
    }
    return consumed;
}

bool window_consume_message_simple(struct gui_window* win, uint16_t event)
{
    struct message msg;
    msg.event = event;
    return window_consume_message(win, &msg);
}

static bool __window_consume(struct gui_window* win, struct message* msg)
{
    for (struct gui_window* w = win; w; w = w->other)
    {
        struct message _msg;
        _msg = *msg;
        if (w->parent)
        {
            _msg.x = msg->x - w->x;
            _msg.y = msg->y - w->y;
        }
        // printf("window_consume %d, %d %d %d %d %d\n", win->id, msg->x, _msg.x, w->x, msg->event, _msg.event);
        if (w->children && __window_consume(w->children, &_msg))
        {
            msg->event = _msg.event;
            return true;
        }
        if (window_consume_message(w, msg))
        {
            return true;
        }
    }
    return false;
}

bool window_consume(struct gui_window* root, struct message* msg)
{
    if (root->dragged && msg->key & MOUSE_LEFT_DRAG)
    {
        msg->event = MESSAGE_MOUSE_DRAG;
        return __gui_window_default_procedure(root, msg);
    }
    __window_consume(root->children, msg);
    return __gui_window_default_procedure(root, msg);
}

void __gui_window_free(struct gui_window* win)
{
    for (struct gui_window* w = win; w;)
    {
        struct gui_window* n = w->other;
        if (w->children)
        {
            __gui_window_free(w->children);
        }
        free(w->title);
        free(w);
        w = n;
    }
}

void gui_window_free(struct gui_window* win)
{
    int id = win->handle;
    win->need_draw = false;
    __gui_window_free(win);
    sys_window_free(id);
}