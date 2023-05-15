#include "../api/ei_draw.h"
#include "../api/ei_widgetclass.h"
#include "ei_impl_widgetclass.h"
#include "ei_application.h"
#include "ei_draw_arc.h"
#include "ei_widget_configure.h"
#include <stdio.h>

ei_widget_t ei_frame_allocfunc()
{
    ei_widget_t frame = (ei_widget_t)calloc(1, sizeof(ei_impl_frame_t));
    return frame;
}

void ei_frame_drawfunc(ei_widget_t frame, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    if (frame->placer_params != NULL)
    {
        ei_impl_placer_run(frame);

        if (((ei_impl_frame_t *)frame)->relief == ei_relief_none || ((ei_impl_frame_t *)frame)->border_width == 0)
        {
            ei_fill(surface, &((ei_impl_frame_t *)frame)->color, clipper);
            if (((ei_impl_frame_t *)frame)->text != NULL)
            {
                ei_draw_frame_text(surface, frame, clipper);
            }
            else if(((ei_impl_frame_t *)frame)->img != NULL)
            {
                ei_draw_frame_img(surface, frame, clipper);
            }
        }
        else
        {
            ei_rect_t inner_clipper;
            inner_clipper.size.height = clipper->size.height - (((ei_impl_frame_t *)frame)->border_width << 1);
            inner_clipper.size.width = clipper->size.width - (((ei_impl_frame_t *)frame)->border_width << 1);
            inner_clipper.top_left.x = clipper->top_left.x + ((ei_impl_frame_t *)frame)->border_width;
            inner_clipper.top_left.y = clipper->top_left.y + ((ei_impl_frame_t *)frame)->border_width;

            ei_color_t lighter_color = ((ei_impl_frame_t *)frame)->color;
            lighter_color.red = (lighter_color.red <= 215) ? (lighter_color.red + 40) : 255;
            lighter_color.green = (lighter_color.green <= 215) ? (lighter_color.green + 40) : 255;
            lighter_color.blue = (lighter_color.blue <= 215) ? (lighter_color.blue + 40) : 255;

            ei_color_t darker_color = ((ei_impl_frame_t *)frame)->color;
            darker_color.red = (darker_color.red >= 40) ? (darker_color.red - 40) : 0;
            darker_color.green = (darker_color.green >= 40) ? (darker_color.green - 40) : 0;
            darker_color.blue = (darker_color.blue >= 40) ? (darker_color.blue - 40) : 0;

            ei_point_t left_border[7];
            left_border[0] = clipper->top_left;
            left_border[1].x = clipper->top_left.x + clipper->size.width;
            left_border[1].y = clipper->top_left.y;
            left_border[2].x = inner_clipper.top_left.x + inner_clipper.size.width;
            left_border[2].y = inner_clipper.top_left.y;
            left_border[3] = inner_clipper.top_left;
            left_border[4].x = inner_clipper.top_left.x;
            left_border[4].y = inner_clipper.top_left.y + inner_clipper.size.height;
            left_border[5].x = clipper->top_left.x;
            left_border[5].y = clipper->top_left.y + clipper->size.height;
            left_border[6] = clipper->top_left;

            ei_point_t right_border[7];
            right_border[0].x = clipper->top_left.x + clipper->size.width;
            right_border[0].y = clipper->top_left.y + clipper->size.height;
            right_border[1].x = clipper->top_left.x + clipper->size.width;
            right_border[1].y = clipper->top_left.y;
            right_border[2].x = inner_clipper.top_left.x + inner_clipper.size.width;
            right_border[2].y = inner_clipper.top_left.y;
            right_border[3].x = inner_clipper.top_left.x + inner_clipper.size.width;
            right_border[3].y = inner_clipper.top_left.y + inner_clipper.size.height;
            right_border[4].x = inner_clipper.top_left.x;
            right_border[4].y = inner_clipper.top_left.y + inner_clipper.size.height;
            right_border[5].x = clipper->top_left.x;
            right_border[5].y = clipper->top_left.y + clipper->size.height;
            right_border[6].x = clipper->top_left.x + clipper->size.width;
            right_border[6].y = clipper->top_left.y + clipper->size.height;

            if (((ei_impl_frame_t *)frame)->relief == ei_relief_raised)
            {
                ei_draw_polygon(surface, left_border, 7, lighter_color, NULL);
                ei_draw_polygon(surface, right_border, 7, darker_color, NULL);
            }
            else
            {
                ei_draw_polygon(surface, left_border, 7, darker_color, NULL);
                ei_draw_polygon(surface, right_border, 7, lighter_color, NULL);
            }
            ei_fill(surface, &((ei_impl_frame_t *)frame)->color, &inner_clipper);
            if (((ei_impl_frame_t *)frame)->text != NULL)
            {
                ei_draw_frame_text(surface, frame, &inner_clipper);
            }
            else if(((ei_impl_frame_t *)frame)->img != NULL)
            {
                ei_draw_frame_img(surface, frame, &inner_clipper);
            }

        }

        // ei_fill(pick_surface, frame->pick_color, &frame->screen_location);
    }
    ei_widget_t child = frame->children_head;
    while (child != NULL)
    {
        (*(child->wclass->drawfunc))(child, surface, pick_surface, &child->screen_location);
        child = child->next_sibling;
    }
}

void ei_frame_setdefaultsfunc(ei_widget_t frame)
{
    // (*(ei_widget_t *)frame)->requested_size = *requested_size;
    ((ei_impl_frame_t *)frame)->color = ei_default_background_color;
    ((ei_impl_frame_t *)frame)->border_width = 0;
    ((ei_impl_frame_t *)frame)->relief = ei_relief_none;
    ((ei_impl_frame_t *)frame)->text = NULL;
    ((ei_impl_frame_t *)frame)->text_font = ei_default_font;
    ((ei_impl_frame_t *)frame)->text_color = ei_font_default_color;
    ((ei_impl_frame_t *)frame)->text_anchor = ei_anc_center;
    ((ei_impl_frame_t *)frame)->img = NULL;
    ((ei_impl_frame_t *)frame)->img_rect = NULL;
    ((ei_impl_frame_t *)frame)->img_anchor = ei_anc_center;
}

void ei_frame_widgetclass_create(ei_widgetclass_t *ei_frame_widgetclass)
{
    ei_frame_widgetclass->allocfunc = &ei_frame_allocfunc;
    ei_frame_widgetclass->releasefunc = NULL;
    ei_frame_widgetclass->drawfunc = &ei_frame_drawfunc;
    ei_frame_widgetclass->setdefaultsfunc = &ei_frame_setdefaultsfunc;
}

void ei_draw_frame_text(ei_surface_t surface, ei_widget_t widget, ei_rect_t *clipper)
{
    int text_width, text_height;
    hw_text_compute_size(((ei_impl_frame_t *)widget)->text, ((ei_impl_frame_t *)widget)->text_font, &text_width, &text_height);
    ei_point_t top_left;
    switch (((ei_impl_frame_t *)widget)->text_anchor)
    {
    case ei_anc_center:
        top_left.x= ((clipper->size.width-text_width)>>1)+clipper->top_left.x;
        top_left.y= ((clipper->size.height-text_height)>>1)+clipper->top_left.y;
        break;
    case ei_anc_north:
        top_left.x= ((clipper->size.width-text_width)>>1)+clipper->top_left.x;
        top_left.y= clipper->top_left.y;
        break;
    case ei_anc_northeast:
        top_left.x= clipper->size.width-text_width+clipper->top_left.x;
        top_left.y= clipper->top_left.y;
        break;
    case ei_anc_east:
        top_left.x= clipper->size.width-text_width+clipper->top_left.x;
        top_left.y= ((clipper->size.height-text_height)>>1)+clipper->top_left.y;
        break;
    case ei_anc_southeast:
        top_left.x= clipper->size.width-text_width+clipper->top_left.x;
        top_left.y= clipper->size.height-text_height+clipper->top_left.y;
        break;
    case ei_anc_south:
        top_left.x= ((clipper->size.width-text_width)>>1)+clipper->top_left.x;
        top_left.y= clipper->size.height-text_height+clipper->top_left.y;
        break;
    case ei_anc_southwest:
        top_left.x= clipper->top_left.x;
        top_left.y= clipper->size.height-text_height+clipper->top_left.y;
        break;
    case ei_anc_west:
        top_left.x= clipper->top_left.x;
        top_left.y= ((clipper->size.height-text_height)>>1)+clipper->top_left.y;
        break;
    case ei_anc_northwest:
        top_left.x= clipper->top_left.x;
        top_left.y= clipper->top_left.y;
        break;
    default:
        break;
    }
    ei_draw_text(surface, &top_left, ((ei_impl_frame_t *)widget)->text, ((ei_impl_frame_t *)widget)->text_font, ((ei_impl_frame_t *)widget)->text_color, clipper);
}

void ei_draw_frame_img(ei_surface_t surface, ei_widget_t widget, ei_rect_t *clipper)
{
    ei_rect_t src_rect;
    ei_rect_t dst_rect;
    if(((ei_impl_frame_t *)widget)->img_rect!=NULL)
    {
        src_rect=*((ei_impl_frame_t *)widget)->img_rect;
    }
    else
    {
        src_rect=hw_surface_get_rect(((ei_impl_frame_t *)widget)->img);
    }
    int img_width=src_rect.size.width;
    int img_height=src_rect.size.height;

    if(img_width>clipper->size.width || img_height>clipper->size.height)
    {
        printf("img_rect to big to fit\n");
        return;
    }

    switch (((ei_impl_frame_t *)widget)->img_anchor)
    {
    case ei_anc_center:
        dst_rect.top_left.x= ((clipper->size.width-img_width)>>1)+clipper->top_left.x;
        dst_rect.top_left.y= ((clipper->size.height-img_height)>>1)+clipper->top_left.y;
        break;
    case ei_anc_north:
        dst_rect.top_left.x= ((clipper->size.width-img_width)>>1)+clipper->top_left.x;
        dst_rect.top_left.y= clipper->top_left.y;
        break;
    case ei_anc_northeast:
        dst_rect.top_left.x= clipper->size.width-img_width+clipper->top_left.x;
        dst_rect.top_left.y= clipper->top_left.y;
        break;
    case ei_anc_east:
        dst_rect.top_left.x= clipper->size.width-img_width+clipper->top_left.x;
        dst_rect.top_left.y= ((clipper->size.height-img_height)>>1)+clipper->top_left.y;
        break;
    case ei_anc_southeast:
        dst_rect.top_left.x= clipper->size.width-img_width+clipper->top_left.x;
        dst_rect.top_left.y= clipper->size.height-img_height+clipper->top_left.y;
        break;
    case ei_anc_south:
        dst_rect.top_left.x= ((clipper->size.width-img_width)>>1)+clipper->top_left.x;
        dst_rect.top_left.y= clipper->size.height-img_height+clipper->top_left.y;
        break;
    case ei_anc_southwest:
        dst_rect.top_left.x= clipper->top_left.x;
        dst_rect.top_left.y= clipper->size.height-img_height+clipper->top_left.y;
        break;
    case ei_anc_west:
        dst_rect.top_left.x= clipper->top_left.x;
        dst_rect.top_left.y= ((clipper->size.height-img_height)>>1)+clipper->top_left.y;
        break;
    case ei_anc_northwest:
        dst_rect.top_left.x= clipper->top_left.x;
        dst_rect.top_left.y= clipper->top_left.y;
        break;
    default:
        break;
    }
    dst_rect.size=src_rect.size;
    ei_copy_surface(surface, &dst_rect, ((ei_impl_frame_t *)widget)->img, &src_rect, true);
}

/* BUTTON */

ei_widget_t ei_button_allocfunc()
{
    ei_impl_widget_t *button = (ei_impl_widget_t *)calloc(1, sizeof(ei_impl_button_t));
    return button;
}

void ei_button_releasefunc(ei_widget_t button)
{
    // free((ei_impl_button_t *)button);
}

void ei_button_setdefaultsfunc(ei_widget_t button)
{
    ((ei_impl_button_t *)button)->color = ei_default_background_color;
    ((ei_impl_button_t *)button)->border_width = k_default_button_border_width;
    ((ei_impl_button_t *)button)->corner_radius = k_default_button_corner_radius;
    ((ei_impl_button_t *)button)->relief = ei_relief_raised;
    ((ei_impl_button_t *)button)->text = NULL;
    ((ei_impl_button_t *)button)->text_font = ei_default_font;
    ((ei_impl_button_t *)button)->text_color = ei_font_default_color;
    ((ei_impl_button_t *)button)->text_anchor = ei_anc_center;
    ((ei_impl_button_t *)button)->img = NULL;
    ((ei_impl_button_t *)button)->img_rect =
     NULL;
    ((ei_impl_button_t *)button)->img_anchor = ei_anc_center;
    ((ei_impl_button_t *)button)->callback = NULL;
    ((ei_impl_button_t *)button)->user_param = NULL;
}

void ei_button_drawfunc(ei_widget_t button,
                        ei_surface_t surface,
                        ei_surface_t pick_surface,
                        ei_rect_t *clipper)
{
    if (button->placer_params != NULL) // show widget on screen iff placer_params is not NULL
    {
        ei_impl_placer_run(button); // calculates the position of widget with regards to the root window and update screen_location of widget

        ei_rect_t rectangle = button->screen_location; 
        ei_draw_button(surface, rectangle, ((ei_impl_button_t *)button)->color, ((ei_impl_button_t *)button)->relief, ((ei_impl_button_t *)button)->border_width, ((ei_impl_button_t *)button)->corner_radius);
    }

 
    if (((ei_impl_button_t *)button)->text != NULL)
    {
        ei_draw_button_text(surface, button, &(button->content_rect));
    }
    else if(((ei_impl_button_t *)button)->img != NULL)
    {
        ei_draw_button_img(surface, button, &(button->content_rect));
    }
    
    
    ei_widget_t children_head = button->children_head;
    while (children_head != NULL)
    {
        (*(children_head->wclass->drawfunc))(children_head, surface, pick_surface, &(button->content_rect));
        children_head = children_head->next_sibling;
    }
}

void ei_draw_button_text(ei_surface_t surface, ei_widget_t widget, ei_rect_t *clipper)
{
    int text_width, text_height;
    hw_text_compute_size(((ei_impl_button_t *)widget)->text, ((ei_impl_button_t *)widget)->text_font, &text_width, &text_height);
    ei_point_t top_left;
    switch (((ei_impl_button_t *)widget)->text_anchor)
    {
    case ei_anc_center:
        top_left.x= ((clipper->size.width-text_width)>>1)+clipper->top_left.x;
        top_left.y= ((clipper->size.height-text_height)>>1)+clipper->top_left.y;
        break;
    case ei_anc_north:
        top_left.x= ((clipper->size.width-text_width)>>1)+clipper->top_left.x;
        top_left.y= clipper->top_left.y;
        break;
    case ei_anc_northeast:
        top_left.x= clipper->size.width-text_width+clipper->top_left.x;
        top_left.y= clipper->top_left.y;
        break;
    case ei_anc_east:
        top_left.x= clipper->size.width-text_width+clipper->top_left.x;
        top_left.y= ((clipper->size.height-text_height)>>1)+clipper->top_left.y;
        break;
    case ei_anc_southeast:
        top_left.x= clipper->size.width-text_width+clipper->top_left.x;
        top_left.y= clipper->size.height-text_height+clipper->top_left.y;
        break;
    case ei_anc_south:
        top_left.x= ((clipper->size.width-text_width)>>1)+clipper->top_left.x;
        top_left.y= clipper->size.height-text_height+clipper->top_left.y;
        break;
    case ei_anc_southwest:
        top_left.x= clipper->top_left.x;
        top_left.y= clipper->size.height-text_height+clipper->top_left.y;
        break;
    case ei_anc_west:
        top_left.x= clipper->top_left.x;
        top_left.y= ((clipper->size.height-text_height)>>1)+clipper->top_left.y;
        break;
    case ei_anc_northwest:
        top_left.x= clipper->top_left.x;
        top_left.y= clipper->top_left.y;
        break;
    default:
        break;
    }
    ei_draw_text(surface, &top_left, ((ei_impl_button_t *)widget)->text, ((ei_impl_button_t *)widget)->text_font, ((ei_impl_button_t *)widget)->text_color, clipper);
}

void ei_draw_button_img(ei_surface_t surface, ei_widget_t widget, ei_rect_t *clipper)
{
    ei_rect_t src_rect;
    ei_rect_t dst_rect;
    if(((ei_impl_button_t *)widget)->img_rect!=NULL)
    {
        src_rect=*((ei_impl_button_t *)widget)->img_rect;
    }
    else
    {
        src_rect=hw_surface_get_rect(((ei_impl_button_t *)widget)->img);
    }
    int img_width=src_rect.size.width;
    int img_height=src_rect.size.height;

    if(img_width>clipper->size.width || img_height>clipper->size.height)
    {
        printf("img_rect to big to fit\n");
        return;
    }

    switch (((ei_impl_frame_t *)widget)->img_anchor)
    {
    case ei_anc_center:
        dst_rect.top_left.x= ((clipper->size.width-img_width)>>1)+clipper->top_left.x;
        dst_rect.top_left.y= ((clipper->size.height-img_height)>>1)+clipper->top_left.y;
        break;
    case ei_anc_north:
        dst_rect.top_left.x= ((clipper->size.width-img_width)>>1)+clipper->top_left.x;
        dst_rect.top_left.y= clipper->top_left.y;
        break;
    case ei_anc_northeast:
        dst_rect.top_left.x= clipper->size.width-img_width+clipper->top_left.x;
        dst_rect.top_left.y= clipper->top_left.y;
        break;
    case ei_anc_east:
        dst_rect.top_left.x= clipper->size.width-img_width+clipper->top_left.x;
        dst_rect.top_left.y= ((clipper->size.height-img_height)>>1)+clipper->top_left.y;
        break;
    case ei_anc_southeast:
        dst_rect.top_left.x= clipper->size.width-img_width+clipper->top_left.x;
        dst_rect.top_left.y= clipper->size.height-img_height+clipper->top_left.y;
        break;
    case ei_anc_south:
        dst_rect.top_left.x= ((clipper->size.width-img_width)>>1)+clipper->top_left.x;
        dst_rect.top_left.y= clipper->size.height-img_height+clipper->top_left.y;
        break;
    case ei_anc_southwest:
        dst_rect.top_left.x= clipper->top_left.x;
        dst_rect.top_left.y= clipper->size.height-img_height+clipper->top_left.y;
        break;
    case ei_anc_west:
        dst_rect.top_left.x= clipper->top_left.x;
        dst_rect.top_left.y= ((clipper->size.height-img_height)>>1)+clipper->top_left.y;
        break;
    case ei_anc_northwest:
        dst_rect.top_left.x= clipper->top_left.x;
        dst_rect.top_left.y= clipper->top_left.y;
        break;
    default:
        break;
    }
    dst_rect.size=src_rect.size;
    ei_copy_surface(surface, &dst_rect, ((ei_impl_button_t *)widget)->img, &src_rect, true);
}

void ei_button_widgetclass_create(ei_widgetclass_t *ei_button_widgetclass)
{
    ei_button_widgetclass->allocfunc = &ei_button_allocfunc;
    ei_button_widgetclass->releasefunc = NULL; // TBC
    ei_button_widgetclass->drawfunc = &ei_button_drawfunc;
    ei_button_widgetclass->setdefaultsfunc = &ei_button_setdefaultsfunc;
}