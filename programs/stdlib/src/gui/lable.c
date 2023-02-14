#include "gui/lable.h"

label_struct* create_window_lable(struct gui_window* parent, uint32_t width, uint32_t height, int32_t x, int32_t y, int id, const char* title, uint32_t color, uint32_t t_color, uint32_t tb_color)
{
    label_struct* label = create_gui_window(parent, width, height, x, y, id, title);
    if (!label) return 0;
    struct screen_buffer* buffer = label->buffer;
    draw_rect_in_absolute_position(parent, x, y, width, height, color);
    gfx_puts((width - TEXT_FONT_WIDTH(title)) / 2, (height - TEXT_FONT_HEIGHT(title)) / 2, t_color, tb_color, title, buffer);
    return label;
}