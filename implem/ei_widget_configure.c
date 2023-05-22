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
        ei_place(frame, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        ei_impl_placer_run(frame);
        ei_app_invalidate_rect(&frame->screen_location);
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
    if (text)
    {
        free(((ei_impl_frame_t*)frame)->text);
        ((ei_impl_frame_t*)frame)->text=strdup(*text);
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
        ((ei_impl_frame_t *)frame)->img = *img;
    }
    if (img_rect)
    {
        ((ei_impl_frame_t *)frame)->img_rect = *img_rect;
    }
    if (img_anchor)
    {
        ((ei_impl_frame_t *)frame)->img_anchor = *img_anchor;
    }
}


void ei_button_configure (ei_widget_t		button,
                            ei_size_t*		requested_size,
                            const ei_color_t*	color,
                            int*			border_width,
                            int*			corner_radius,
                            ei_relief_t*		relief,
                            ei_string_t*		text,
                            ei_font_t*		text_font,
                            ei_color_t*		text_color,
                            ei_anchor_t*		text_anchor,
                            ei_surface_t*		img,
                            ei_rect_ptr_t*		img_rect,
                            ei_anchor_t*		img_anchor,
                            ei_callback_t*		callback,
                            ei_user_param_t*	user_param)
{
    if(requested_size)
    {
        button->requested_size = *requested_size;
        ei_place(button, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        ei_impl_placer_run(button);
        ei_app_invalidate_rect(&button->screen_location);
    }
    if(color)
    {
        ((ei_impl_button_t*) button)->color = *color;
    }
    if(border_width)
    {
        ((ei_impl_button_t*) button)->border_width = *border_width;
    }
    if(corner_radius)
    {
        ((ei_impl_button_t*) button)->corner_radius = *corner_radius;
    }
    if(relief)
    {
        ((ei_impl_button_t*) button)->relief = *relief;
    }
    if(text)
    {
        free(((ei_impl_button_t*)button)->text);
        ((ei_impl_button_t*)button)->text=strdup(*text);
    }
    if(text_font)
    {
        ((ei_impl_button_t*) button)->text_font = *text_font;
    }
    if(text_color)
    {
        ((ei_impl_button_t*) button)->text_color = *text_color;
    }
    if(text_anchor)
    {
        ((ei_impl_button_t*) button)->text_anchor = *text_anchor;
    }
    if(img)
    {
        ((ei_impl_button_t*) button)->img = *img;
    }
    if(img_rect)
    {
        ((ei_impl_button_t*) button)->img_rect = *img_rect;
    }
    if(img_anchor)
    {
        ((ei_impl_button_t*) button)->img_anchor = *img_anchor;
    }
    if(callback)
    {
        ((ei_impl_button_t*) button)->callback = *callback;
    }
    if(user_param)
    {
        ((ei_impl_button_t*) button)->user_param = *user_param;
    }
}

void ei_toplevel_configure(ei_widget_t		widget,
							 ei_size_t*		requested_size,
							 ei_color_t*		color,
							 int*			border_width,
							 ei_string_t*		title,
							 bool*			closable,
							 ei_axis_set_t*		resizable,
						 	 ei_size_ptr_t*		min_size)
{
    if(requested_size)
    {
        widget->requested_size = *requested_size;
        ei_place(widget, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        ei_impl_placer_run(widget);
        ei_app_invalidate_rect(&widget->screen_location);
    }
    if(color)
    {
        ((ei_impl_toplevel_t *)widget)->color=*color;
    }
    if(border_width)
    {
        ((ei_impl_toplevel_t *)widget)->border_width=*border_width;
    }
    if(title)
    {
        if(((ei_impl_toplevel_t*)widget)->title!=NULL)
        {
            free(((ei_impl_toplevel_t*)widget)->title);
        }
        ((ei_impl_toplevel_t*)widget)->title=strdup(*title);
    }
    if(closable)
    {
        ((ei_impl_toplevel_t *)widget)->closable=*closable;
    }
    if(resizable)
    {
        ((ei_impl_toplevel_t *)widget)->resizable=*resizable;
    }
    if(min_size)
    {
        ((ei_impl_toplevel_t *)widget)->min_size=*min_size;
    }
}