#include "../api/ei_widget_configure.h"
#include "../api/ei_widgetclass.h"
#include "ei_implementation.h"
#include "ei_widgetclass.h"
#include "ei_impl_widgetclass.h"
#include "../api/ei_application.h"

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
        frame->requested_size = *requested_size;
        /* in case requested_size is used for width and height */
        ei_place(frame, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    if (color)
    {
        ((ei_impl_frame_t *)frame)->color = *color;
    }
    if (border_width)
    {
        ((ei_impl_frame_t *)frame)->border_width = *border_width;
    }
    if (relief)
    {
        ((ei_impl_frame_t *)frame)->relief = *relief;
    }
    if (text && *text)
    {
        free(((ei_impl_frame_t *)frame)->text);
        ((ei_impl_frame_t *)frame)->text = strdup(*text);
    }
    if (text_font)
    {
        ((ei_impl_frame_t *)frame)->text_font = *text_font;
    }
    if (text_color)
    {
        ((ei_impl_frame_t *)frame)->text_color = *text_color;
    }
    if (text_anchor)
    {
        ((ei_impl_frame_t *)frame)->text_anchor = *text_anchor;
    }
    if (img)
    {
        if(((ei_impl_frame_t *)frame)->img)
        {
            hw_surface_free(((ei_impl_frame_t *)frame)->img);
        }
        ((ei_impl_frame_t *)frame)->img = hw_surface_create(ei_app_root_surface(), hw_surface_get_size(*img), true);
        hw_surface_lock(*img);
        hw_surface_lock(((ei_impl_frame_t *)frame)->img);
        ei_copy_surface(((ei_impl_frame_t *)frame)->img, NULL, *img, NULL, true);
        hw_surface_unlock(*img);
        hw_surface_unlock(((ei_impl_frame_t *)frame)->img);
    }
    if (img_rect)
    {
        free(((ei_impl_frame_t *)frame)->img_rect);
        ((ei_impl_frame_t *)frame)->img_rect = malloc(sizeof(ei_rect_t));
        *(((ei_impl_frame_t *)frame)->img_rect) = **img_rect;
    }
    if (img_anchor)
    {
        ((ei_impl_frame_t *)frame)->img_anchor = *img_anchor;
    }
    /* to invalide the right rect */
    ei_impl_placer_run(frame);
    ei_app_invalidate_rect(&frame->screen_location);
}

void ei_button_configure(ei_widget_t button,
                         ei_size_t *requested_size,
                         const ei_color_t *color,
                         int *border_width,
                         int *corner_radius,
                         ei_relief_t *relief,
                         ei_string_t *text,
                         ei_font_t *text_font,
                         ei_color_t *text_color,
                         ei_anchor_t *text_anchor,
                         ei_surface_t *img,
                         ei_rect_ptr_t *img_rect,
                         ei_anchor_t *img_anchor,
                         ei_callback_t *callback,
                         ei_user_param_t *user_param)
{
    if (requested_size)
    {
        button->requested_size = *requested_size;
        /* in case requested_size is used for width and height */
        ei_place(button, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    if (color)
    {
        ((ei_impl_button_t *)button)->color = *color;
    }
    if (border_width)
    {
        ((ei_impl_button_t *)button)->border_width = *border_width;
    }
    if (corner_radius)
    {
        ((ei_impl_button_t *)button)->corner_radius = *corner_radius;
    }
    if (relief)
    {
        ((ei_impl_button_t *)button)->relief = *relief;
    }
    if (text)
    {
        free(((ei_impl_button_t *)button)->text);
        ((ei_impl_button_t *)button)->text = strdup(*text);
    }
    if (text_font)
    {
        ((ei_impl_button_t *)button)->text_font = *text_font;
    }
    if (text_color)
    {
        ((ei_impl_button_t *)button)->text_color = *text_color;
    }
    if (text_anchor)
    {
        ((ei_impl_button_t *)button)->text_anchor = *text_anchor;
    }
    if (img)
    {
        if(((ei_impl_button_t *)button)->img)
        {
            hw_surface_free(((ei_impl_button_t *)button)->img);
        }
        ((ei_impl_button_t *)button)->img = hw_surface_create(ei_app_root_surface(), hw_surface_get_size(*img), true);
        hw_surface_lock(*img);
        hw_surface_lock(((ei_impl_button_t *)button)->img);
        ei_copy_surface(((ei_impl_button_t *)button)->img, NULL, *img, NULL, true);
        hw_surface_unlock(*img);
        hw_surface_unlock(((ei_impl_button_t *)button)->img);
    }
    if (img_rect)
    {
        free(((ei_impl_button_t *)button)->img_rect);
        ((ei_impl_button_t *)button)->img_rect = malloc(sizeof(ei_rect_t));
        *(((ei_impl_button_t *)button)->img_rect) = **img_rect;
    }
    if (img_anchor)
    {
        ((ei_impl_button_t *)button)->img_anchor = *img_anchor;
    }
    if (callback)
    {
        ((ei_impl_button_t *)button)->callback = *callback;
    }
    if (user_param)
    {
        ((ei_impl_button_t *)button)->user_param = *user_param;
    }
    /* to invalide the right rect */
    ei_impl_placer_run(button);
    ei_app_invalidate_rect(&button->screen_location);
}

void ei_toplevel_configure(ei_widget_t toplevel,
                           ei_size_t *requested_size,
                           ei_color_t *color,
                           int *border_width,
                           ei_string_t *title,
                           bool *closable,
                           ei_axis_set_t *resizable,
                           ei_size_ptr_t *min_size)
{
    if (requested_size)
    {
        toplevel->requested_size = *requested_size;
        /* in case requested_size is used for width and height */
        ei_place(toplevel, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    if (color)
    {
        ((ei_impl_toplevel_t *)toplevel)->color = *color;
    }
    if (border_width)
    {
        ((ei_impl_toplevel_t *)toplevel)->border_width = *border_width;
    }
    if (title)
    {
        if (((ei_impl_toplevel_t *)toplevel)->title != NULL)
        {
            free(((ei_impl_toplevel_t *)toplevel)->title);
        }
        ((ei_impl_toplevel_t *)toplevel)->title = strdup(*title);
    }
    if (closable)
    {
        ((ei_impl_toplevel_t *)toplevel)->closable = *closable;
    }
    if (resizable)
    {
        ((ei_impl_toplevel_t *)toplevel)->resizable = *resizable;
    }
    if (min_size)
    {
        ((ei_impl_toplevel_t *)toplevel)->min_size = malloc(sizeof(ei_size_t));
        *(((ei_impl_toplevel_t *)toplevel)->min_size) = **min_size;
    }
    /* to invalide the right rect */
    ei_impl_placer_run(toplevel);
    ei_app_invalidate_rect(&toplevel->screen_location);
}