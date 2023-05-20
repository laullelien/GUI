#include "../api/ei_placer.h"
#include "ei_implementation.h"
#include "ei_impl_widgetclass.h"
#include "ei_application.h"


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
        if (anchor == NULL)
        {
            widget->placer_params->anchor = ei_anc_northwest;
        }
    }

    if (anchor)
    {
        widget->placer_params->anchor = *anchor;
    }

    if (x)
    {
        widget->placer_params->x = *x;
    }

    if (y)
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
                /* default value in case the widget should display an image or some text */
                if (strcmp("frame", widget->wclass->name) == 0)
                {
                    if (((ei_impl_frame_t *)widget)->img_rect)
                    {
                        widget->placer_params->width = ((ei_impl_frame_t *)widget)->img_rect->size.width;
                    }
                    else if (((ei_impl_frame_t *)widget)->text)
                    {
                        hw_text_compute_size(((ei_impl_frame_t *)widget)->text, ((ei_impl_frame_t *)widget)->text_font, &widget->placer_params->width, NULL);
                    }
                }
                else if (strcmp("button", widget->wclass->name) == 0)
                {
                    if (((ei_impl_button_t *)widget)->img_rect)
                    {
                        widget->placer_params->width = ((ei_impl_button_t *)widget)->img_rect->size.width;
                    }
                    else if (((ei_impl_button_t *)widget)->text)
                    {
                        hw_text_compute_size(((ei_impl_button_t *)widget)->text, ((ei_impl_button_t *)widget)->text_font, &widget->placer_params->width, NULL);
                    }
                }
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
                /* default value in case the widget should display an image or some text */
                if (strcmp("frame", widget->wclass->name) == 0)
                {
                    if (((ei_impl_frame_t *)widget)->img_rect)
                    {
                        widget->placer_params->height = ((ei_impl_frame_t *)widget)->img_rect->size.height;
                    }
                    else if (((ei_impl_frame_t *)widget)->text)
                    {
                        hw_text_compute_size(((ei_impl_frame_t *)widget)->text, ((ei_impl_frame_t *)widget)->text_font, NULL, &widget->placer_params->height);
                    }
                }
                else if (strcmp("button", widget->wclass->name) == 0)
                {
                    if (((ei_impl_button_t *)widget)->img_rect)
                    {
                        widget->placer_params->height = ((ei_impl_button_t *)widget)->img_rect->size.height;
                    }
                    else if (((ei_impl_button_t *)widget)->text)
                    {
                        hw_text_compute_size(((ei_impl_button_t *)widget)->text, ((ei_impl_button_t *)widget)->text_font, NULL, &widget->placer_params->height);
                    }
                }
            }
        }
    }
    else
    {
        widget->placer_params->height = *height;
    }

    if (rel_x)
    {
        widget->placer_params->rel_x = *rel_x;
    }

    if (rel_y)
    {
        widget->placer_params->rel_y = *rel_y;
    }

    if (rel_width)
    {
        widget->placer_params->rel_width = *rel_width;
    }

    if (rel_height)
    {
        widget->placer_params->rel_height = *rel_height;
    }
    ei_impl_placer_run(widget);
    ei_app_invalidate_rect(&widget->screen_location);
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

void ei_placer_forget(ei_widget_t widget)
{
    free(widget->placer_params);
    widget->placer_params = NULL;
}