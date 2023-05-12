#include "../api/ei_placer.h"
#include "ei_implementation.h"

void ei_place(ei_widget_t widget,
              ei_anchor_t *anchor,
              int *x,
              int *y,
              int *width,
              int *height,
              float *rel_x,
              float *rel_y,
              float *rel_width,
              float *rel_height)
{

    if (widget->placer_params == NULL)
    {
        widget->placer_params = calloc(1, sizeof(ei_impl_placer_params_t));
    }

    if (anchor == NULL)
    {
        widget->placer_params->anchor = ei_anc_northwest;
    }
    else
    {
        widget->placer_params->anchor = *anchor;
    }

    if (x != NULL)
    {
        widget->placer_params->x = *x;
    }

    if (y != NULL)
    {
        widget->placer_params->y = *y;
    }

    if (width == NULL)
    {
        if (rel_width == NULL)
        {

            if (widget->requested_size.width != 0)
            {
                widget->placer_params->width = widget->requested_size.width;
            }
            else
            {
                // widget->placer_params->width = (int) widget->parent->screen_location.size.width/2;
            }
        }
    }
    else
    {
        widget->placer_params->width = *width;
    }

    if (height == NULL)
    {
        if (rel_height == NULL)
        {

            if (widget->requested_size.height != 0)
            {
                widget->placer_params->height = widget->requested_size.height;
            }
            else
            {
                // widget->placer_params->height = (int) widget->parent->screen_location.size.height/2;
            }
        }
    }
    else
    {
        widget->placer_params->height = *height;
    }

    if (rel_x != NULL)
    {
        widget->placer_params->rel_x = *rel_x;
    }

    if (rel_y != NULL)
    {
        widget->placer_params->rel_y = *rel_y;
    }

    if (rel_width != NULL)
    {
        widget->placer_params->rel_width = *rel_width;
    }

    if (rel_height != NULL)
    {
        widget->placer_params->rel_height = *rel_height;
    }
}

void ei_impl_placer_run(ei_widget_t widget)
{
    if (widget->placer_params == NULL || widget->parent == NULL)
    {
        return;
    }
    ei_rect_t parent_rect = *(widget->parent->content_rect);

    ei_point_t anchor_coord;
    anchor_coord.x = widget->placer_params->rel_x * parent_rect.size.width + widget->placer_params->x + widget->parent->content_rect->top_left.x;
    anchor_coord.y = widget->placer_params->rel_y * parent_rect.size.height + widget->placer_params->y + widget->parent->content_rect->top_left.y;

    int widget_width = widget->placer_params->rel_width * widget->parent->content_rect->size.width + widget->placer_params->width;
    int widget_height = widget->placer_params->rel_height * widget->parent->content_rect->size.height + widget->placer_params->height;
    widget->screen_location.size.width = widget_width;
    widget->screen_location.size.height = widget_height;

    switch (widget->placer_params->anchor)
    {

    case ei_anc_center:
        widget->screen_location.top_left.x = anchor_coord.x - (widget_width << 1);
        widget->screen_location.top_left.y = anchor_coord.y - (widget_height << 1);
        break;
    case ei_anc_north:
        widget->screen_location.top_left.x = anchor_coord.x - (widget_width << 1);
        widget->screen_location.top_left.y = anchor_coord.y;
        break;
    case ei_anc_northeast:
        widget->screen_location.top_left.x = anchor_coord.x - widget_width;
        widget->screen_location.top_left.y = anchor_coord.y;
        break;
    case ei_anc_east:
        widget->screen_location.top_left.x = anchor_coord.x - widget_width;
        widget->screen_location.top_left.y = anchor_coord.y - (widget_height << 1);
        break;
    case ei_anc_southeast:
        widget->screen_location.top_left.x = anchor_coord.x - widget_width;
        widget->screen_location.top_left.y = anchor_coord.y - widget_height;
        break;
    case ei_anc_south:
        widget->screen_location.top_left.x = anchor_coord.x - (widget_width << 1);
        widget->screen_location.top_left.y = anchor_coord.y - widget_height;
        break;
    case ei_anc_southwest:
        widget->screen_location.top_left.x = anchor_coord.x;
        widget->screen_location.top_left.y = anchor_coord.y - widget_height;
        break;
    case ei_anc_west:
        widget->screen_location.top_left.x = anchor_coord.x;
        widget->screen_location.top_left.y = anchor_coord.y - (widget_height << 1);
        break;
    case ei_anc_northwest:
        widget->screen_location.top_left.x = anchor_coord.x;
        widget->screen_location.top_left.y = anchor_coord.y;
        break;
    default:
        break;
    }
}

void ei_placer_forget(ei_widget_t widget)
{
    free(widget->placer_params);
    widget->placer_params = NULL;
}