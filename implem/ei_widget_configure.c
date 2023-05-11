#include "../api/ei_widget_configure.h"
#include "../api/ei_widgetclass.h"
#include "ei_implementation.h"
#include "ei_widgetclass.h"

void ei_frame_configure(ei_widget_t frame,
                        ei_size_t *requested_size,
                        const ei_color_t *color,
                        int *border_width,
                        ei_relief_t *relief,
                        ei_string_t *text,
                        ei_font_t *text_font,
                        ei_color_t *text_color,
                        ei_anchor_t *text_anchor,
                        ei_surface_t *img,
                        ei_rect_ptr_t *img_rect,
                        ei_anchor_t *img_anchor)
{
    if (requested_size)
    {
        (*(ei_widget_t *)frame)->requested_size = *requested_size;
    }
    if (color)
    {
        ((ei_frame_t *)frame)->color = *color;
    }
    if (border_width)
    {
        ((ei_frame_t *)frame)->border_width = *border_width;
    }
    if (relief)
    {
        ((ei_frame_t *)frame)->relief = *relief;
    }
    if (text)
    {
        ((ei_frame_t *)frame)->text = *text;
    }
    if (text_font)
    {
        ((ei_frame_t *)frame)->text_font = *text_font;
    }
    if (text_color)
    {
        ((ei_frame_t *)frame)->text_color = *text_color;
    }
    if (text_anchor)
    {
        ((ei_frame_t *)frame)->text_anchor = *text_anchor;
    }
    if (img)
    {
        ((ei_frame_t *)frame)->img = *img;
    }
    if (img_rect)
    {
        ((ei_frame_t *)frame)->img_rect = *img_rect;
    }
    if (img_anchor)
    {
        ((ei_frame_t *)frame)->img_anchor = *img_anchor;
    }
}
