#include "ei_impl_widgetclass.h"
#include "ei_implementation.h"

#include <stdint.h>

static int ir, ig, ib;

uint32_t ei_impl_map_rgba(ei_surface_t surface, ei_color_t color)
{
    int ia;
    hw_surface_get_channel_indices(surface, &ir, &ig, &ib, &ia);
    uint8_t pixel_color[4];
    pixel_color[ir] = color.red;
    pixel_color[ig] = color.green;
    pixel_color[ib] = color.blue;
    if (ia != -1)
    {
        pixel_color[ia] = color.alpha;
    }
    return *((uint32_t *)pixel_color);
}

uint32_t ei_get_id(ei_surface_t surface, uint32_t *color)
{
    /* ir will always be initiated by ei_impl_map_rgba*/
    return (((uint8_t *)color)[ir] << 16) + (((uint8_t *)color)[ig] << 8) + (((uint8_t *)color)[ib]);
}

void ei_impl_placer_run(ei_widget_t widget)
{
    if (widget->placer_params == NULL)
    {
        return;
    }

    ei_point_t anchor_coord;
    int widget_width;
    int widget_height;

    if (widget->parent)
    {
        ei_rect_t parent_rect = widget->parent->content_rect;

        anchor_coord.x = widget->placer_params->rel_x * parent_rect.size.width + widget->placer_params->x + parent_rect.top_left.x;
        anchor_coord.y = widget->placer_params->rel_y * parent_rect.size.height + widget->placer_params->y + parent_rect.top_left.y;
        widget_width = widget->placer_params->rel_width * parent_rect.size.width + widget->placer_params->width;
        widget_height = widget->placer_params->rel_height * parent_rect.size.height + widget->placer_params->height;

        widget->content_rect.size.width = widget_width;
        widget->content_rect.size.height = widget_height;
    }
    else
    {
        anchor_coord.x = widget->placer_params->x;
        anchor_coord.y = widget->placer_params->y;

        widget_width = widget->content_rect.size.width;
        widget_height = widget->content_rect.size.height;
    }
    /* set content_rect */
    switch (widget->placer_params->anchor)
    {

    case ei_anc_center:
        widget->content_rect.top_left.x = anchor_coord.x - (widget_width >> 1);
        widget->content_rect.top_left.y = anchor_coord.y - (widget_height >> 1);
        break;
    case ei_anc_north:
        widget->content_rect.top_left.x = anchor_coord.x - (widget_width >> 1);
        widget->content_rect.top_left.y = anchor_coord.y;
        break;
    case ei_anc_northeast:
        widget->content_rect.top_left.x = anchor_coord.x - widget_width;
        widget->content_rect.top_left.y = anchor_coord.y;
        break;
    case ei_anc_east:
        widget->content_rect.top_left.x = anchor_coord.x - widget_width;
        widget->content_rect.top_left.y = anchor_coord.y - (widget_height >> 1);
        break;
    case ei_anc_southeast:
        widget->content_rect.top_left.x = anchor_coord.x - widget_width;
        widget->content_rect.top_left.y = anchor_coord.y - widget_height;
        break;
    case ei_anc_south:
        widget->content_rect.top_left.x = anchor_coord.x - (widget_width >> 1);
        widget->content_rect.top_left.y = anchor_coord.y - widget_height;
        break;
    case ei_anc_southwest:
        widget->content_rect.top_left.x = anchor_coord.x;
        widget->content_rect.top_left.y = anchor_coord.y - widget_height;
        break;
    case ei_anc_west:
        widget->content_rect.top_left.x = anchor_coord.x;
        widget->content_rect.top_left.y = anchor_coord.y - (widget_height >> 1);
        break;
    case ei_anc_northwest:
        widget->content_rect.top_left.x = anchor_coord.x;
        widget->content_rect.top_left.y = anchor_coord.y;
        break;
    default:
        break;
    }
    /* set screen_location */
    if (strcmp("frame", widget->wclass->name) == 0)
    {
        widget->screen_location.top_left.x = widget->content_rect.top_left.x - ((ei_impl_frame_t *)widget)->border_width;
        widget->screen_location.top_left.y = widget->content_rect.top_left.y - ((ei_impl_frame_t *)widget)->border_width;
        widget->screen_location.size.width = widget->content_rect.size.width + (((ei_impl_frame_t *)widget)->border_width << 1);
        widget->screen_location.size.height = widget->content_rect.size.height + (((ei_impl_frame_t *)widget)->border_width << 1);
    }
    else if (strcmp("button", widget->wclass->name) == 0)
    {
        widget->screen_location.top_left.x = widget->content_rect.top_left.x - ((ei_impl_button_t *)widget)->corner_radius;
        widget->screen_location.top_left.y = widget->content_rect.top_left.y - ((ei_impl_button_t *)widget)->corner_radius;
        widget->screen_location.size.width = widget->content_rect.size.width + (((ei_impl_button_t *)widget)->corner_radius << 1);
        widget->screen_location.size.height = widget->content_rect.size.height + (((ei_impl_button_t *)widget)->corner_radius << 1);
    }
    else if (strcmp("toplevel", widget->wclass->name) == 0)
    {
        int text_height;
        hw_text_compute_size(((ei_impl_toplevel_t *)widget)->title, ei_default_font, NULL, &text_height);
        widget->screen_location.top_left.x = widget->content_rect.top_left.x - ((ei_impl_toplevel_t *)widget)->border_width;
        widget->screen_location.top_left.y = widget->content_rect.top_left.y - (((ei_impl_toplevel_t *)widget)->border_width << 1) - text_height;
        widget->screen_location.size.width = widget->content_rect.size.width + (((ei_impl_toplevel_t *)widget)->border_width << 1);
        widget->screen_location.size.height = widget->content_rect.size.height + text_height + 3 * ((ei_impl_toplevel_t *)widget)->border_width;
    }
}
