#include "gui/gui.h"
#include "gui/label.h"
#include "window/window.h"
#include "string.h"

label_struct* create_window_label(struct gui_window* parent, uint32_t width, uint32_t height, int32_t x, int32_t y, int32_t z, int id, const char* title, uint16_t attributes, uint32_t color, uint32_t t_color, uint32_t tb_color, pixels_producteur producteur)
{
    label_struct* label = create_gui_window(parent, width, height, x, y, z, id, title, attributes, producteur);
    if (!label) return 0;
    struct screen_buffer* buffer = label->buffer;
    draw_rect_in_absolute_position(label, x, y, width, height, color);
    if (title)   gfx_puts((width - TEXT_FONT_WIDTH(title)) / 2, (height - TEXT_FONT_HEIGHT(title)) / 2, t_color, tb_color, title, buffer);
    return label;
}