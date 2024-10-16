#include <stdio.h>
#include "../api/ei_draw.h"
#include "../api/ei_widgetclass.h"
#include "ei_impl_widgetclass.h"
#include "ei_impl_draw.h"
#include "ei_application.h"
#include "ei_draw_arc.h"
#include "ei_widget_configure.h"
#include "ei_event.h"

static ei_point_t ancient_mouse_location;
static int is_top_toplevel = 0; // 0 == false; 1 == true;
static int is_small_square_toplevel = 0;



void external_class_register(ei_widgetclass_t* widgetclass)
{
    ei_widgetclass_register(widgetclass);
}









/* FRAME */

ei_widget_t ei_frame_allocfunc()
{
    ei_widget_t frame = (ei_widget_t)calloc(1, sizeof(ei_impl_frame_t));
    return frame;
}

void ei_frame_releasefunc(ei_widget_t frame)
{
    free(frame->placer_params);
    free(((ei_impl_frame_t *)frame)->text);
    if (((ei_impl_frame_t *)frame)->img)
    {
        hw_surface_free(((ei_impl_frame_t *)frame)->img);
    }
    free(((ei_impl_frame_t *)frame)->img_rect);
}
void ei_frame_drawfunc(ei_widget_t frame, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    ei_impl_placer_run(frame);
    ei_rect_t *screen_location_intersection = ei_intersect_clipper(clipper, &frame->screen_location);
    if (screen_location_intersection != NULL && frame->placer_params != NULL)
    {
        ei_rect_t *content_rect_intersection = ei_intersect_clipper(clipper, &frame->content_rect);

        if (((ei_impl_frame_t *)frame)->relief == ei_relief_none || ((ei_impl_frame_t *)frame)->border_width == 0)
        {
            ei_point_t frame_points[5];
            frame_points[0] = frame->screen_location.top_left;
            frame_points[1].x = frame->screen_location.top_left.x + frame->screen_location.size.width;
            frame_points[1].y = frame_points[0].y;
            frame_points[2].x = frame_points[1].x;
            frame_points[2].y = frame_points[1].y + frame->screen_location.size.height;
            frame_points[3].x = frame_points[0].x;
            frame_points[3].y = frame_points[2].y;
            frame_points[4] = frame->screen_location.top_left;

            ei_draw_polygon(surface, frame_points, 5, ((ei_impl_frame_t *)frame)->color, screen_location_intersection);
            ei_draw_polygon(pick_surface, frame_points, 5, (ei_color_t){(uint8_t)(frame->pick_id >> 16), (uint8_t)(frame->pick_id >> 8), (uint8_t)(frame->pick_id), 0}, screen_location_intersection);

            if (((ei_impl_frame_t *)frame)->text != NULL && content_rect_intersection != NULL)
            {
                ei_draw_frame_text(surface, frame, content_rect_intersection);
            }
            else if (((ei_impl_frame_t *)frame)->img != NULL && content_rect_intersection != NULL)
            {
                ei_draw_frame_img(surface, frame, content_rect_intersection);
            }
        }
        else
        {

            ei_color_t lighter_color = ((ei_impl_frame_t *)frame)->color;
            lighter_color.red = (lighter_color.red <= 215) ? (lighter_color.red + 40) : 255;
            lighter_color.green = (lighter_color.green <= 215) ? (lighter_color.green + 40) : 255;
            lighter_color.blue = (lighter_color.blue <= 215) ? (lighter_color.blue + 40) : 255;

            ei_color_t darker_color = ((ei_impl_frame_t *)frame)->color;
            darker_color.red = (darker_color.red >= 40) ? (darker_color.red - 40) : 0;
            darker_color.green = (darker_color.green >= 40) ? (darker_color.green - 40) : 0;
            darker_color.blue = (darker_color.blue >= 40) ? (darker_color.blue - 40) : 0;

            ei_point_t left_border[7];
            left_border[0] = frame->screen_location.top_left;
            left_border[1].x = frame->screen_location.top_left.x + frame->screen_location.size.width;
            left_border[1].y = frame->screen_location.top_left.y;
            left_border[2].x = frame->content_rect.top_left.x + frame->content_rect.size.width;
            left_border[2].y = frame->content_rect.top_left.y;
            left_border[3] = frame->content_rect.top_left;
            left_border[4].x = frame->content_rect.top_left.x;
            left_border[4].y = frame->content_rect.top_left.y + frame->content_rect.size.height;
            left_border[5].x = frame->screen_location.top_left.x;
            left_border[5].y = frame->screen_location.top_left.y + frame->screen_location.size.height;
            left_border[6] = frame->screen_location.top_left;

            ei_point_t right_border[7];
            right_border[0].x = frame->screen_location.top_left.x + frame->screen_location.size.width;
            right_border[0].y = frame->screen_location.top_left.y + frame->screen_location.size.height;
            right_border[1].x = frame->screen_location.top_left.x + frame->screen_location.size.width;
            right_border[1].y = frame->screen_location.top_left.y;
            right_border[2].x = frame->content_rect.top_left.x + frame->content_rect.size.width;
            right_border[2].y = frame->content_rect.top_left.y;
            right_border[3].x = frame->content_rect.top_left.x + frame->content_rect.size.width;
            right_border[3].y = frame->content_rect.top_left.y + frame->content_rect.size.height;
            right_border[4].x = frame->content_rect.top_left.x;
            right_border[4].y = frame->content_rect.top_left.y + frame->content_rect.size.height;
            right_border[5].x = frame->screen_location.top_left.x;
            right_border[5].y = frame->screen_location.top_left.y + frame->screen_location.size.height;
            right_border[6].x = frame->screen_location.top_left.x + frame->screen_location.size.width;
            right_border[6].y = frame->screen_location.top_left.y + frame->screen_location.size.height;

            if (((ei_impl_frame_t *)frame)->relief == ei_relief_raised)
            {
                ei_draw_polygon(surface, left_border, 7, lighter_color, screen_location_intersection);
                ei_draw_polygon(surface, right_border, 7, darker_color, screen_location_intersection);
            }
            else
            {
                ei_draw_polygon(surface, left_border, 7, darker_color, screen_location_intersection);
                ei_draw_polygon(surface, right_border, 7, lighter_color, screen_location_intersection);
            }
            if (content_rect_intersection != NULL)
            {
                ei_point_t frame_points[5];
                frame_points[0] = frame->content_rect.top_left;
                frame_points[1].x = frame->content_rect.top_left.x + frame->content_rect.size.width;
                frame_points[1].y = frame_points[0].y;
                frame_points[2].x = frame_points[1].x;
                frame_points[2].y = frame_points[1].y + frame->content_rect.size.height;
                frame_points[3].x = frame_points[0].x;
                frame_points[3].y = frame_points[2].y;
                frame_points[4] = frame->content_rect.top_left;
                ei_draw_polygon(surface, frame_points, 5, ((ei_impl_frame_t *)frame)->color, content_rect_intersection);
                ei_draw_polygon(pick_surface, frame_points, 5, (ei_color_t){(uint8_t)(frame->pick_id >> 16), (uint8_t)(frame->pick_id >> 8), (uint8_t)(frame->pick_id), 0}, content_rect_intersection);

                if (((ei_impl_frame_t *)frame)->text != NULL)
                {
                    ei_draw_frame_text(surface, frame, content_rect_intersection);
                }
                else if (((ei_impl_frame_t *)frame)->img != NULL)
                {
                    ei_draw_frame_img(surface, frame, content_rect_intersection);
                }
            }
        }
        /* draw children */
        if (content_rect_intersection != NULL)
        {
            ei_widget_t child = frame->children_head;
            while (child != NULL)
            {
                (*(child->wclass->drawfunc))(child, surface, pick_surface, content_rect_intersection);
                child = child->next_sibling;
            }
        }
        free(content_rect_intersection);
    }
    free(screen_location_intersection);
}

void ei_frame_setdefaultsfunc(ei_widget_t frame)
{
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
    ei_frame_widgetclass->releasefunc = &ei_frame_releasefunc;
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
        top_left.x = ((widget->content_rect.size.width - text_width) >> 1) + widget->content_rect.top_left.x;
        top_left.y = ((widget->content_rect.size.height - text_height) >> 1) + widget->content_rect.top_left.y;
        break;
    case ei_anc_north:
        top_left.x = ((widget->content_rect.size.width - text_width) >> 1) + widget->content_rect.top_left.x;
        top_left.y = widget->content_rect.top_left.y;
        break;
    case ei_anc_northeast:
        top_left.x = widget->content_rect.size.width - text_width + widget->content_rect.top_left.x;
        top_left.y = widget->content_rect.top_left.y;
        break;
    case ei_anc_east:
        top_left.x = widget->content_rect.size.width - text_width + widget->content_rect.top_left.x;
        top_left.y = ((widget->content_rect.size.height - text_height) >> 1) + widget->content_rect.top_left.y;
        break;
    case ei_anc_southeast:
        top_left.x = widget->content_rect.size.width - text_width + widget->content_rect.top_left.x;
        top_left.y = widget->content_rect.size.height - text_height + widget->content_rect.top_left.y;
        break;
    case ei_anc_south:
        top_left.x = ((widget->content_rect.size.width - text_width) >> 1) + widget->content_rect.top_left.x;
        top_left.y = widget->content_rect.size.height - text_height + widget->content_rect.top_left.y;
        break;
    case ei_anc_southwest:
        top_left.x = widget->content_rect.top_left.x;
        top_left.y = widget->content_rect.size.height - text_height + widget->content_rect.top_left.y;
        break;
    case ei_anc_west:
        top_left.x = widget->content_rect.top_left.x;
        top_left.y = ((widget->content_rect.size.height - text_height) >> 1) + widget->content_rect.top_left.y;
        break;
    case ei_anc_northwest:
        top_left.x = widget->content_rect.top_left.x;
        top_left.y = widget->content_rect.top_left.y;
        break;
    default:
        break;
    }
    ei_draw_text(surface, &top_left, ((ei_impl_frame_t *)widget)->text, ((ei_impl_frame_t *)widget)->text_font, ((ei_impl_frame_t *)widget)->text_color, clipper);
}

void ei_draw_frame_img(ei_surface_t surface, ei_widget_t widget, ei_rect_t *clipper)
{
    ei_point_t top_left;
    int img_width, img_height;
    ei_rect_t src_rect;

    /* we define the image width and height */
    if (((ei_impl_frame_t *)widget)->img_rect != NULL)
    {
        img_width = ((ei_impl_frame_t *)widget)->img_rect->size.width;
        img_height = ((ei_impl_frame_t *)widget)->img_rect->size.height;
        src_rect.top_left = ((ei_impl_frame_t *)widget)->img_rect->top_left;
    }
    else
    {
        ei_rect_t img_rect = hw_surface_get_rect(((ei_impl_frame_t *)widget)->img);
        img_width = img_rect.size.width;
        img_height = img_rect.size.height;
        src_rect.top_left = (ei_point_t){0, 0};
    }

    /* We define the topleft coordinates that the image should have if it fits in the clipper */
    switch (((ei_impl_frame_t *)widget)->img_anchor)
    {
    case ei_anc_center:
        top_left.x = ((widget->content_rect.size.width - img_width) >> 1) + widget->content_rect.top_left.x;
        top_left.y = ((widget->content_rect.size.height - img_height) >> 1) + widget->content_rect.top_left.y;
        break;
    case ei_anc_north:
        top_left.x = ((widget->content_rect.size.width - img_width) >> 1) + widget->content_rect.top_left.x;
        top_left.y = widget->content_rect.top_left.y;
        break;
    case ei_anc_northeast:
        top_left.x = widget->content_rect.size.width - img_width + widget->content_rect.top_left.x;
        top_left.y = widget->content_rect.top_left.y;
        break;
    case ei_anc_east:
        top_left.x = widget->content_rect.size.width - img_width + widget->content_rect.top_left.x;
        top_left.y = ((widget->content_rect.size.height - img_height) >> 1) + widget->content_rect.top_left.y;
        break;
    case ei_anc_southeast:
        top_left.x = widget->content_rect.size.width - img_width + widget->content_rect.top_left.x;
        top_left.y = widget->content_rect.size.height - img_height + widget->content_rect.top_left.y;
        break;
    case ei_anc_south:
        top_left.x = ((widget->content_rect.size.width - img_width) >> 1) + widget->content_rect.top_left.x;
        top_left.y = widget->content_rect.size.height - img_height + widget->content_rect.top_left.y;
        break;
    case ei_anc_southwest:
        top_left.x = widget->content_rect.top_left.x;
        top_left.y = widget->content_rect.size.height - img_height + widget->content_rect.top_left.y;
        break;
    case ei_anc_west:
        top_left.x = widget->content_rect.top_left.x;
        top_left.y = ((widget->content_rect.size.height - img_height) >> 1) + widget->content_rect.top_left.y;
        break;
    case ei_anc_northwest:
        top_left.x = widget->content_rect.top_left.x;
        top_left.y = widget->content_rect.top_left.y;
        break;
    default:
        break;
    }

    ei_rect_t dst_rect;
    dst_rect.top_left = top_left;
    dst_rect.size.width = img_width;
    dst_rect.size.width = img_height;

    if (clipper != NULL)
    {
        /* intersect clipper and dst_rect and apply same modifications on src_rect */
        if (dst_rect.size.width + dst_rect.top_left.x > clipper->size.width + clipper->top_left.x)
        {
            dst_rect.size.width = - dst_rect.top_left.x + clipper->size.width + clipper->top_left.x;
        }
        if (dst_rect.size.height + dst_rect.top_left.y > clipper->size.height + clipper->top_left.y)
        {
            dst_rect.size.height = - dst_rect.top_left.y + clipper->size.height + clipper->top_left.y;
        }
        if (dst_rect.top_left.x < clipper->top_left.x)
        {
            dst_rect.size.width -= clipper->top_left.x - dst_rect.top_left.x;
            src_rect.top_left.x += clipper->top_left.x - dst_rect.top_left.x;
            dst_rect.top_left.x = clipper->top_left.x;
        }
        if (dst_rect.top_left.y < clipper->top_left.y)
        {
            dst_rect.size.height -= clipper->top_left.y - dst_rect.top_left.y;
            src_rect.top_left.y += clipper->top_left.y - dst_rect.top_left.y;
            dst_rect.top_left.y = clipper->top_left.y;
        }
    }
    src_rect.size = dst_rect.size;

    hw_surface_lock(((ei_impl_frame_t *)widget)->img);
    if (dst_rect.size.width > 0 && dst_rect.size.height > 0)
    {
        ei_copy_surface(surface, &dst_rect, ((ei_impl_frame_t *)widget)->img, &src_rect, true);
    }
    hw_surface_unlock(((ei_impl_frame_t *)widget)->img);
}

/* BUTTON */

ei_widget_t ei_button_allocfunc()
{
    ei_impl_widget_t *button = (ei_impl_widget_t *)calloc(1, sizeof(ei_impl_button_t));
    return button;
}

void ei_button_releasefunc(ei_widget_t button)
{
    free(button->placer_params);
    free(((ei_impl_button_t *)button)->text);
    if (((ei_impl_button_t *)button)->img)
    {
        hw_surface_free(((ei_impl_button_t *)button)->img);
    }
    free(((ei_impl_button_t *)button)->img_rect);
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
    ((ei_impl_button_t *)button)->img_rect = NULL;
    ((ei_impl_button_t *)button)->img_anchor = ei_anc_center;
    ((ei_impl_button_t *)button)->callback = NULL;
    ((ei_impl_button_t *)button)->user_param = NULL;
}

void ei_button_drawfunc(ei_widget_t button,
                        ei_surface_t surface,
                        ei_surface_t pick_surface,
                        ei_rect_t *clipper)
{
    ei_impl_placer_run(button);
    ei_rect_t *screen_location_intersection = ei_intersect_clipper(clipper, &button->screen_location);
    if (screen_location_intersection != NULL && button->placer_params != NULL)
    {
        ei_rect_t *content_rect_intersection = ei_intersect_clipper(clipper, &button->content_rect);

        ei_rect_t rectangle = button->screen_location;

        ei_draw_button(surface, rectangle, ((ei_impl_button_t *)button)->color, ((ei_impl_button_t *)button)->relief, ((ei_impl_button_t *)button)->border_width, ((ei_impl_button_t *)button)->corner_radius, screen_location_intersection, pick_surface, (ei_color_t){(uint8_t)(button->pick_id >> 16), (uint8_t)(button->pick_id >> 8), (uint8_t)(button->pick_id), 0});

        if (content_rect_intersection != NULL)
        {
            ei_rect_t f_rect;
            f_rect = *content_rect_intersection;
            f_rect.size.width = f_rect.size.width / 2;
            f_rect.size.height = f_rect.size.height / 2;
            ei_rect_t f_rect_2 = f_rect;
            f_rect_2.top_left.x += f_rect.size.width;
            f_rect_2.top_left.y += f_rect.size.height;

            if (((ei_impl_button_t *)button)->text != NULL)
            {
                ei_draw_button_text(surface, button, content_rect_intersection);
            }
            else if (((ei_impl_button_t *)button)->img != NULL)
            {
                ei_draw_button_img(surface, button, content_rect_intersection);
            }

            ei_widget_t children_head = button->children_head;
            while (children_head != NULL)
            {
                (*(children_head->wclass->drawfunc))(children_head, surface, pick_surface, content_rect_intersection);
                children_head = children_head->next_sibling;
            }
        }
        free(content_rect_intersection);
    }
    free(screen_location_intersection);
}

void ei_draw_button_text(ei_surface_t surface, ei_widget_t widget, ei_rect_t *clipper)
{
    int text_width, text_height;
    hw_text_compute_size(((ei_impl_button_t *)widget)->text, ((ei_impl_button_t *)widget)->text_font, &text_width, &text_height);
    ei_point_t top_left;
    switch (((ei_impl_button_t *)widget)->text_anchor)
    {
    case ei_anc_center:
        top_left.x = ((widget->content_rect.size.width - text_width) >> 1) + widget->content_rect.top_left.x;
        top_left.y = ((widget->content_rect.size.height - text_height) >> 1) + widget->content_rect.top_left.y;
        break;
    case ei_anc_north:
        top_left.x = ((widget->content_rect.size.width - text_width) >> 1) + widget->content_rect.top_left.x;
        top_left.y = widget->content_rect.top_left.y;
        break;
    case ei_anc_northeast:
        top_left.x = widget->content_rect.size.width - text_width + widget->content_rect.top_left.x;
        top_left.y = widget->content_rect.top_left.y;
        break;
    case ei_anc_east:
        top_left.x = widget->content_rect.size.width - text_width + widget->content_rect.top_left.x;
        top_left.y = ((widget->content_rect.size.height - text_height) >> 1) + widget->content_rect.top_left.y;
        break;
    case ei_anc_southeast:
        top_left.x = widget->content_rect.size.width - text_width + widget->content_rect.top_left.x;
        top_left.y = widget->content_rect.size.height - text_height + widget->content_rect.top_left.y;
        break;
    case ei_anc_south:
        top_left.x = ((widget->content_rect.size.width - text_width) >> 1) + widget->content_rect.top_left.x;
        top_left.y = widget->content_rect.size.height - text_height + widget->content_rect.top_left.y;
        break;
    case ei_anc_southwest:
        top_left.x = widget->content_rect.top_left.x;
        top_left.y = widget->content_rect.size.height - text_height + widget->content_rect.top_left.y;
        break;
    case ei_anc_west:
        top_left.x = widget->content_rect.top_left.x;
        top_left.y = ((widget->content_rect.size.height - text_height) >> 1) + widget->content_rect.top_left.y;
        break;
    case ei_anc_northwest:
        top_left.x = widget->content_rect.top_left.x;
        top_left.y = widget->content_rect.top_left.y;
        break;
    default:
        break;
    }
    ei_draw_text(surface, &top_left, ((ei_impl_button_t *)widget)->text, ((ei_impl_button_t *)widget)->text_font, ((ei_impl_button_t *)widget)->text_color, clipper);
}

void ei_draw_button_img(ei_surface_t surface, ei_widget_t widget, ei_rect_t *clipper)
{
    ei_point_t top_left;
    int img_width, img_height;
    ei_rect_t src_rect;
    
    /* we define the image width and height */
    if (((ei_impl_frame_t *)widget)->img_rect != NULL)
    {
        img_width = ((ei_impl_frame_t *)widget)->img_rect->size.width;
        img_height = ((ei_impl_frame_t *)widget)->img_rect->size.height;
        src_rect.top_left = ((ei_impl_frame_t *)widget)->img_rect->top_left;
    }
    else
    {
        ei_rect_t img_rect = hw_surface_get_rect(((ei_impl_frame_t *)widget)->img);
        img_width = img_rect.size.width;
        img_height = img_rect.size.height;
        src_rect.top_left = (ei_point_t){0, 0};
    }

    /* We define the topleft coordinates that the image should have if it fits in the clipper */
    switch (((ei_impl_button_t *)widget)->img_anchor)
    {
    case ei_anc_center:
        top_left.x = ((widget->content_rect.size.width - img_width) >> 1) + widget->content_rect.top_left.x;
        top_left.y = ((widget->content_rect.size.height - img_height) >> 1) + widget->content_rect.top_left.y;
        break;
    case ei_anc_north:
        top_left.x = ((widget->content_rect.size.width - img_width) >> 1) + widget->content_rect.top_left.x;
        top_left.y = widget->content_rect.top_left.y;
        break;
    case ei_anc_northeast:
        top_left.x = widget->content_rect.size.width - img_width + widget->content_rect.top_left.x;
        top_left.y = widget->content_rect.top_left.y;
        break;
    case ei_anc_east:
        top_left.x = widget->content_rect.size.width - img_width + widget->content_rect.top_left.x;
        top_left.y = ((widget->content_rect.size.height - img_height) >> 1) + widget->content_rect.top_left.y;
        break;
    case ei_anc_southeast:
        top_left.x = widget->content_rect.size.width - img_width + widget->content_rect.top_left.x;
        top_left.y = widget->content_rect.size.height - img_height + widget->content_rect.top_left.y;
        break;
    case ei_anc_south:
        top_left.x = ((widget->content_rect.size.width - img_width) >> 1) + widget->content_rect.top_left.x;
        top_left.y = widget->content_rect.size.height - img_height + widget->content_rect.top_left.y;
        break;
    case ei_anc_southwest:
        top_left.x = widget->content_rect.top_left.x;
        top_left.y = widget->content_rect.size.height - img_height + widget->content_rect.top_left.y;
        break;
    case ei_anc_west:
        top_left.x = widget->content_rect.top_left.x;
        top_left.y = ((widget->content_rect.size.height - img_height) >> 1) + widget->content_rect.top_left.y;
        break;
    case ei_anc_northwest:
        top_left.x = widget->content_rect.top_left.x;
        top_left.y = widget->content_rect.top_left.y;
        break;
    default:
        break;
    }

    ei_rect_t dst_rect;
    dst_rect.top_left = top_left;
    dst_rect.size.width = img_width;
    dst_rect.size.width = img_height;

    if (clipper != NULL)
    {
        /* intersect clipper and dst_rect and apply same modifications on src_rect */
        if (dst_rect.size.width + dst_rect.top_left.x > clipper->size.width + clipper->top_left.x)
        {
            dst_rect.size.width = - dst_rect.top_left.x + clipper->size.width + clipper->top_left.x;
        }
        if (dst_rect.size.height + dst_rect.top_left.y > clipper->size.height + clipper->top_left.y)
        {
            dst_rect.size.height = - dst_rect.top_left.y + clipper->size.height + clipper->top_left.y;
        }
        if (dst_rect.top_left.x < clipper->top_left.x)
        {
            dst_rect.size.width -= clipper->top_left.x - dst_rect.top_left.x;
            src_rect.top_left.x += clipper->top_left.x - dst_rect.top_left.x;
            dst_rect.top_left.x = clipper->top_left.x;
        }
        if (dst_rect.top_left.y < clipper->top_left.y)
        {
            dst_rect.size.height -= clipper->top_left.y - dst_rect.top_left.y;
            src_rect.top_left.y += clipper->top_left.y - dst_rect.top_left.y;
            dst_rect.top_left.y = clipper->top_left.y;
        }
    }
    src_rect.size = dst_rect.size;

    hw_surface_lock(((ei_impl_button_t *)widget)->img);
    if (dst_rect.size.width > 0 && dst_rect.size.height > 0)
    {
        ei_copy_surface(surface, &dst_rect, ((ei_impl_button_t *)widget)->img, &src_rect, true);
    }
    hw_surface_unlock(((ei_impl_button_t *)widget)->img);
}

bool ei_button_handlefunc(ei_widget_t widget, struct ei_event_t *event)
{
    if (event->type == ei_ev_mouse_buttondown)
    {
        ei_event_set_active_widget(widget);
        if (((ei_impl_button_t *)widget)->relief == ei_relief_raised)
        {
            ((ei_impl_button_t *)widget)->relief = ei_relief_sunken;
            ei_app_invalidate_rect(&widget->screen_location);
        }
        else if (((ei_impl_button_t *)widget)->relief == ei_relief_sunken)
        {
            ((ei_impl_button_t *)widget)->relief = ei_relief_raised;
            ei_app_invalidate_rect(&widget->screen_location);
        }
        return true;
    }
    else if (event->type == ei_ev_mouse_buttonup && ei_event_get_active_widget() == widget)
    {
        /* set widget inactive */
        ei_event_set_active_widget(NULL);
        if (((ei_impl_button_t *)widget)->relief == ei_relief_sunken)
        {
            ((ei_impl_button_t *)widget)->relief = ei_relief_raised;
            ei_app_invalidate_rect(&widget->screen_location);
        }
        else if (((ei_impl_button_t *)widget)->relief == ei_relief_raised)
        {
            ((ei_impl_button_t *)widget)->relief = ei_relief_sunken;
            ei_app_invalidate_rect(&widget->screen_location);
        }
        if (((ei_impl_button_t *)widget)->callback != NULL)
        {
            (*(((ei_impl_button_t *)widget)->callback))(widget, event, ((ei_impl_button_t *)widget)->user_param);
        }
        return true;
    }
    return false;
}

void ei_button_widgetclass_create(ei_widgetclass_t *ei_button_widgetclass)
{
    ei_button_widgetclass->allocfunc = &ei_button_allocfunc;
    ei_button_widgetclass->releasefunc = &ei_button_releasefunc; // TBC
    ei_button_widgetclass->drawfunc = &ei_button_drawfunc;
    ei_button_widgetclass->setdefaultsfunc = &ei_button_setdefaultsfunc;
    ei_button_widgetclass->handlefunc = &ei_button_handlefunc;
}

/* TOP LEVEL */

ei_widget_t ei_toplevel_allocfunc()
{
    ei_widget_t toplevel = (ei_widget_t)calloc(1, sizeof(ei_impl_toplevel_t));
    return toplevel;
}

void ei_toplevel_releasefunc(ei_widget_t toplevel)
{
    free(toplevel->placer_params);
    free(((ei_impl_toplevel_t *)toplevel)->title);
    free(((ei_impl_toplevel_t *)toplevel)->min_size);
}

void ei_toplevel_drawfunc(ei_widget_t toplevel,
                          ei_surface_t surface,
                          ei_surface_t pick_surface,
                          ei_rect_t *clipper)
{
    ei_impl_placer_run(toplevel);
    ei_rect_t *screen_location_intersection = ei_intersect_clipper(clipper, &toplevel->screen_location);
    if (screen_location_intersection != NULL && toplevel->placer_params != NULL)
    {
        ei_rect_t *content_rect_intersection = ei_intersect_clipper(clipper, &toplevel->content_rect);

        /* bottom part of the border */
        ei_point_t bottom_border[11];
        bottom_border[0] = toplevel->content_rect.top_left;
        bottom_border[1].x = toplevel->content_rect.top_left.x;
        bottom_border[1].y = toplevel->content_rect.top_left.y + toplevel->content_rect.size.height;
        bottom_border[2].x = toplevel->content_rect.top_left.x + toplevel->content_rect.size.width;
        bottom_border[2].y = bottom_border[1].y;
        bottom_border[3].x = bottom_border[2].x;
        bottom_border[3].y = toplevel->content_rect.top_left.y;
        bottom_border[4].x = bottom_border[3].x + ((ei_impl_toplevel_t *)toplevel)->border_width;
        bottom_border[4].y = bottom_border[3].y;
        bottom_border[5].x = bottom_border[2].x + ((ei_impl_toplevel_t *)toplevel)->border_width;
        bottom_border[5].y = bottom_border[2].y + ((ei_impl_toplevel_t *)toplevel)->border_width;
        bottom_border[6].x = toplevel->screen_location.top_left.x;
        bottom_border[6].y = bottom_border[1].y + ((ei_impl_toplevel_t *)toplevel)->border_width;
        bottom_border[7].x = toplevel->screen_location.top_left.x;
        bottom_border[7].y = toplevel->content_rect.top_left.y;
        bottom_border[8] = bottom_border[0];

        ei_color_t border_color = {100, 100, 100, 255};
        ei_draw_polygon(surface, bottom_border, 9, border_color, screen_location_intersection);
        ei_draw_polygon(pick_surface, bottom_border, 9, (ei_color_t){(uint8_t)(toplevel->pick_id >> 16), (uint8_t)(toplevel->pick_id >> 8), (uint8_t)(toplevel->pick_id), 0}, screen_location_intersection);

        /* upper part of the border */
        int text_height;
        hw_text_compute_size(((ei_impl_toplevel_t *)toplevel)->title, ei_default_font, NULL, &text_height);
        int upper_border_height = text_height + (((ei_impl_toplevel_t *)toplevel)->border_width << 1);
        int close_button_radius = text_height / 3;
        int window_corner_radius = upper_border_height >> 1;

        ei_point_t close_button_center = {toplevel->screen_location.top_left.x + (upper_border_height >> 1), toplevel->screen_location.top_left.y + (upper_border_height >> 1)};
        ei_point_t upper_right_corner_center = {toplevel->screen_location.top_left.x + toplevel->screen_location.size.width - window_corner_radius, toplevel->screen_location.top_left.y + (upper_border_height >> 1)};
        ei_point_t upper_border[7 + (((int)(window_corner_radius * 0.70710678118 + 1)) << 2)];

        upper_border[0].x = toplevel->screen_location.top_left.x;
        upper_border[0].y = toplevel->screen_location.top_left.y + upper_border_height;
        upper_border[1].x = toplevel->screen_location.top_left.x + toplevel->screen_location.size.width;
        upper_border[1].y = upper_border[0].y;
        upper_border[2].x = upper_border[1].x;
        upper_border[2].y = upper_border[1].y - (upper_border_height >> 1);

        int length;
        list_of_points_for_an_arc(upper_right_corner_center, window_corner_radius, 0, M_PI / 2, &length, upper_border + 3);

        upper_border[length + 3].x = toplevel->screen_location.top_left.x + toplevel->screen_location.size.width - window_corner_radius;
        upper_border[length + 3].y = toplevel->screen_location.top_left.y;
        upper_border[length + 4].x = toplevel->screen_location.top_left.x + window_corner_radius;
        upper_border[length + 4].y = toplevel->screen_location.top_left.y;

        list_of_points_for_an_arc(close_button_center, window_corner_radius, M_PI / 2, M_PI, &length, upper_border + length + 5);

        upper_border[2 * length + 5].x = toplevel->screen_location.top_left.x;
        upper_border[2 * length + 5].y = toplevel->screen_location.top_left.y + window_corner_radius;
        upper_border[2 * length + 6] = upper_border[0];

        ei_draw_polygon(surface, upper_border, 2 * length + 7, border_color, screen_location_intersection);
        ei_draw_polygon(pick_surface, upper_border, 2 * length + 7, (ei_color_t){(uint8_t)(toplevel->pick_id >> 16), (uint8_t)(toplevel->pick_id >> 8), (uint8_t)(toplevel->pick_id), 0}, screen_location_intersection);

        /* close button */
        if (((ei_impl_toplevel_t *)toplevel)->closable)
        {
            ei_rect_t button_square;
            button_square.top_left.x = close_button_center.x - close_button_radius;
            button_square.top_left.y = close_button_center.y - close_button_radius;
            button_square.size.width = close_button_radius << 1;
            button_square.size.height = close_button_radius << 1;
            ei_draw_button(surface, button_square, (ei_color_t){200, 0, 0, 255}, ei_relief_raised, 2, close_button_radius, screen_location_intersection, pick_surface, (ei_color_t){(uint8_t)(toplevel->pick_id >> 16), (uint8_t)(toplevel->pick_id >> 8), (uint8_t)(toplevel->pick_id), 0});
        }
        /* title */
        ei_point_t title_top_left = {toplevel->screen_location.top_left.x + (window_corner_radius << 1), toplevel->screen_location.top_left.y + ((ei_impl_toplevel_t *)toplevel)->border_width};
        ei_draw_text(surface, &title_top_left, ((ei_impl_toplevel_t *)toplevel)->title, ei_default_font, (ei_color_t){255, 255, 255, 255}, screen_location_intersection);
        ei_widget_t child = toplevel->children_head;

        if (content_rect_intersection != NULL)
        {
            /* content rectangle */
            ei_point_t content_rect_points[5];
            content_rect_points[0] = toplevel->content_rect.top_left;
            content_rect_points[1].x = toplevel->content_rect.top_left.x + toplevel->content_rect.size.width;
            content_rect_points[1].y = content_rect_points[0].y;
            content_rect_points[2].x = content_rect_points[1].x;
            content_rect_points[2].y = content_rect_points[1].y + toplevel->content_rect.size.height;
            content_rect_points[3].x = content_rect_points[0].x;
            content_rect_points[3].y = content_rect_points[2].y;
            content_rect_points[4] = toplevel->content_rect.top_left;
            ei_draw_polygon(surface, content_rect_points, 5, ((ei_impl_toplevel_t *)toplevel)->color, content_rect_intersection);
            ei_draw_polygon(pick_surface, content_rect_points, 5, (ei_color_t){(uint8_t)(toplevel->pick_id >> 16), (uint8_t)(toplevel->pick_id >> 8), (uint8_t)(toplevel->pick_id), 0}, content_rect_intersection);

            while (child != NULL)
            {
                (*(child->wclass->drawfunc))(child, surface, pick_surface, content_rect_intersection);
                child = child->next_sibling;
            }
        }
        if (((ei_impl_toplevel_t *)toplevel)->resizable)
        {
            /* resize square */
            ei_point_t resize_square[5];
            resize_square[0].x = bottom_border[2].x - 10;
            resize_square[0].y = bottom_border[2].y - 10;
            resize_square[1].x = bottom_border[2].x + 1;
            resize_square[1].y = resize_square[0].y;
            resize_square[2].x = bottom_border[2].x + 1;
            resize_square[2].y = bottom_border[2].y + 1;
            resize_square[3].x = resize_square[0].x;
            resize_square[3].y = bottom_border[2].y + 1;
            resize_square[4] = resize_square[0];
            ei_draw_polygon(surface, resize_square, 5, border_color, screen_location_intersection);
            ei_draw_polygon(pick_surface, resize_square, 5, (ei_color_t){(uint8_t)(toplevel->pick_id >> 16), (uint8_t)(toplevel->pick_id >> 8), (uint8_t)(toplevel->pick_id), 0}, screen_location_intersection);
        }

        free(content_rect_intersection);
    }
    free(screen_location_intersection);
}

void ei_toplevel_setdefaultsfunc(ei_widget_t toplevel)
{
    toplevel->requested_size = (ei_size_t){320, 240};
    ((ei_impl_toplevel_t *)toplevel)->color = ei_default_background_color;
    ((ei_impl_toplevel_t *)toplevel)->border_width = 4;
    ((ei_impl_toplevel_t *)toplevel)->title = malloc(9 * sizeof(char));
    strncpy(((ei_impl_toplevel_t *)toplevel)->title, "Toplevel", 9 * sizeof(char));
    ((ei_impl_toplevel_t *)toplevel)->closable = true;
    ((ei_impl_toplevel_t *)toplevel)->resizable = ei_axis_both;
    ((ei_impl_toplevel_t *)toplevel)->min_size = malloc(sizeof(ei_size_t));
    *(((ei_impl_toplevel_t *)toplevel)->min_size) = (ei_size_t){160, 120};
}

bool ei_toplevel_handlefunc(ei_widget_t widget,
                            struct ei_event_t *event)
{

    // verify if there is an active widget
    if (ei_event_get_active_widget() != NULL && event->type == ei_ev_mouse_move)
    {
        int delta_x, delta_y;
        delta_x = event->param.mouse.where.x - ancient_mouse_location.x;
        delta_y = event->param.mouse.where.y - ancient_mouse_location.y;

        // if we have clicked on the top of the toplevel
        if (is_top_toplevel == 1 && is_small_square_toplevel == 0)
        {
            widget->placer_params->x += delta_x;
            widget->placer_params->y += delta_y;
            ancient_mouse_location = event->param.mouse.where;
        }
        // if we have clicked on the small square at the bottom right corner of the toplevel
        else if (is_top_toplevel == 0 && is_small_square_toplevel == 1)
        {
            int final_width, final_height;
            final_width = widget->placer_params->width + delta_x;
            final_height = widget->placer_params->height + delta_y;
            if (((ei_impl_toplevel_t *)widget)->resizable != ei_axis_both || ((ei_impl_toplevel_t *)widget)->resizable != ei_axis_x)
            {
                if (final_width >= ((ei_impl_toplevel_t *)widget)->min_size->width)
                {
                    widget->placer_params->width += delta_x;
                }
                else
                {
                    widget->placer_params->width = ((ei_impl_toplevel_t *)widget)->min_size->width;
                }
            }
            if (((ei_impl_toplevel_t *)widget)->resizable != ei_axis_both || ((ei_impl_toplevel_t *)widget)->resizable != ei_axis_y)
            {
                if (final_height >= ((ei_impl_toplevel_t *)widget)->min_size->height)
                {
                    widget->placer_params->height += delta_y;
                }
                else
                {
                    widget->placer_params->height = ((ei_impl_toplevel_t *)widget)->min_size->height;
                }
            }
            ancient_mouse_location = event->param.mouse.where;
        }

        // update the screen by adding the rectangles into invalide rect list
        ei_rect_t root_rect = hw_surface_get_rect(ei_app_root_surface());
        ei_rect_t *prev_screen_location_intersection = ei_intersect_clipper(&widget->screen_location, &root_rect);
        ei_app_invalidate_rect(prev_screen_location_intersection);
        // calculate the new rect and put inside the function
        ei_impl_placer_run(widget);
        ei_rect_t *current_screen_location_intersection = ei_intersect_clipper(&widget->screen_location, &root_rect);
        ei_app_invalidate_rect(current_screen_location_intersection);
        free(prev_screen_location_intersection);
        free(current_screen_location_intersection);
        return true;
    }

    // if the mouse button is released
    else if (event->type == ei_ev_mouse_buttonup && event->param.mouse.button == ei_mouse_button_left)
    {
        ei_event_set_active_widget(NULL);
        return true;
    }

    else
    {
        /* when there's no active widget */

        if (event->type == ei_ev_mouse_buttondown && event->param.mouse.button == ei_mouse_button_left)
        {
            int height_text, width_text;
            hw_text_compute_size(((ei_impl_toplevel_t *)widget)->title, ei_default_font, &width_text, &height_text);
            int height_top_of_the_toplevel = height_text + (((ei_impl_toplevel_t *)widget)->border_width << 1);

            // verify that mouse click is on the top of toplevel
            if (event->param.mouse.where.y < (widget->screen_location.top_left.y + height_top_of_the_toplevel))
            {
                int radius_close_button = height_text / 3;
                ei_point_t centre_close_button = {widget->screen_location.top_left.x + (height_top_of_the_toplevel >> 1), widget->screen_location.top_left.y + (height_top_of_the_toplevel >> 1)};

                // if we have clicked on the close button
                if (abs(event->param.mouse.where.x - centre_close_button.x) <= radius_close_button && abs(event->param.mouse.where.y - centre_close_button.y) <= radius_close_button && ((ei_impl_toplevel_t *)widget)->closable)
                {
                    ei_widget_destroy(widget);
                    return true;
                }
                // if we have clicked on the other parts of the top level to deplace it
                else
                {
                    ei_event_set_active_widget(widget);
                    is_top_toplevel = 1;
                    is_small_square_toplevel = 0;
                    ancient_mouse_location = event->param.mouse.where;
                    return true;
                }
            }

            // verify that mouse click is in the small square at the botton right corner

            else if (((ei_impl_toplevel_t *)widget)->resizable != ei_axis_none &&
                     (event->param.mouse.where.x <= (widget->screen_location.top_left.x + widget->screen_location.size.width)) &&
                     (event->param.mouse.where.x >= (widget->screen_location.top_left.x + widget->screen_location.size.width - (15 + ((ei_impl_toplevel_t *)widget)->border_width))) &&
                     (event->param.mouse.where.y <= (widget->screen_location.top_left.y + widget->screen_location.size.height)) &&
                     (event->param.mouse.where.y >= (widget->screen_location.top_left.y + widget->screen_location.size.height - (15 + ((ei_impl_toplevel_t *)widget)->border_width))))
            {
                ei_event_set_active_widget(widget);
                is_top_toplevel = 0;
                is_small_square_toplevel = 1;
                ancient_mouse_location = event->param.mouse.where;
                return true;
            }
        }
    }
    return false;
}

void ei_toplevel_widgetclass_create(ei_widgetclass_t *ei_toplevel_widgetclass)
{
    ei_toplevel_widgetclass->allocfunc = &ei_toplevel_allocfunc;
    ei_toplevel_widgetclass->releasefunc = &ei_toplevel_releasefunc;
    ei_toplevel_widgetclass->drawfunc = &ei_toplevel_drawfunc;
    ei_toplevel_widgetclass->setdefaultsfunc = &ei_toplevel_setdefaultsfunc;
    ei_toplevel_widgetclass->handlefunc = &ei_toplevel_handlefunc;
}
