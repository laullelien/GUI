#include "../api/ei_draw.h"
#include "../api/ei_widgetclass.h"
#include "ei_impl_widgetclass.h"
#include "ei_application.h"
#include "ei_draw_arc.h"
#include "ei_widget_configure.h"

ei_widget_t ei_frame_allocfunc()
{
    ei_widget_t frame = (ei_widget_t)calloc(1, sizeof(ei_impl_frame_t));
    return frame;
}

void ei_frame_releasefunc(ei_widget_t frame)
{
    if (frame->placer_params != NULL)
    {
        free(frame->placer_params);
    }
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
            else if (((ei_impl_frame_t *)frame)->img != NULL)
            {
                ei_draw_frame_img(surface, frame, clipper);
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
            left_border[0] = clipper->top_left;
            left_border[1].x = clipper->top_left.x + clipper->size.width;
            left_border[1].y = clipper->top_left.y;
            left_border[2].x = frame->content_rect.top_left.x + frame->content_rect.size.width;
            left_border[2].y = frame->content_rect.top_left.y;
            left_border[3] = frame->content_rect.top_left;
            left_border[4].x = frame->content_rect.top_left.x;
            left_border[4].y = frame->content_rect.top_left.y + frame->content_rect.size.height;
            left_border[5].x = clipper->top_left.x;
            left_border[5].y = clipper->top_left.y + clipper->size.height;
            left_border[6] = clipper->top_left;

            ei_point_t right_border[7];
            right_border[0].x = clipper->top_left.x + clipper->size.width;
            right_border[0].y = clipper->top_left.y + clipper->size.height;
            right_border[1].x = clipper->top_left.x + clipper->size.width;
            right_border[1].y = clipper->top_left.y;
            right_border[2].x = frame->content_rect.top_left.x + frame->content_rect.size.width;
            right_border[2].y = frame->content_rect.top_left.y;
            right_border[3].x = frame->content_rect.top_left.x + frame->content_rect.size.width;
            right_border[3].y = frame->content_rect.top_left.y + frame->content_rect.size.height;
            right_border[4].x = frame->content_rect.top_left.x;
            right_border[4].y = frame->content_rect.top_left.y + frame->content_rect.size.height;
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
            ei_fill(surface, &((ei_impl_frame_t *)frame)->color, &frame->content_rect);
            if (((ei_impl_frame_t *)frame)->text != NULL)
            {
                ei_draw_frame_text(surface, frame, &frame->content_rect);
            }
            else if (((ei_impl_frame_t *)frame)->img != NULL)
            {
                ei_draw_frame_img(surface, frame, &frame->content_rect);
            }
        }

        // ei_fill(pick_surface, frame->pick_color, &frame->screen_location);

        ei_widget_t child = frame->children_head;
        while (child != NULL)
        {
            (*(child->wclass->drawfunc))(child, surface, pick_surface, &child->screen_location);
            child = child->next_sibling;
        }
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
    ei_frame_widgetclass->releasefunc = &ei_frame_releasefunc;
    ei_frame_widgetclass->drawfunc = &ei_frame_drawfunc;
    ei_frame_widgetclass->setdefaultsfunc = &ei_frame_setdefaultsfunc;
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
    ei_rect_t rectangle;
    rectangle = button->screen_location;

    ei_draw_button(surface, rectangle, ((ei_impl_button_t *)button)->color, ((ei_impl_button_t *)button)->relief, ((ei_impl_button_t *)button)->border_width, ((ei_impl_button_t *)button)->corner_radius);
}

void ei_draw_frame_text(ei_surface_t surface, ei_widget_t widget, ei_rect_t *clipper)
{
    int text_width, text_height;
    hw_text_compute_size(((ei_impl_frame_t *)widget)->text, ((ei_impl_frame_t *)widget)->text_font, &text_width, &text_height);
    ei_point_t topl_left;
    switch (((ei_impl_frame_t *)widget)->text_anchor)
    {
    case ei_anc_center:
        topl_left.x = ((clipper->size.width - text_width) >> 1) + clipper->top_left.x;
        topl_left.y = ((clipper->size.height - text_height) >> 1) + clipper->top_left.y;
        break;
    case ei_anc_north:
        topl_left.x = ((clipper->size.width - text_width) >> 1) + clipper->top_left.x;
        topl_left.y = clipper->top_left.y;
        break;
    case ei_anc_northeast:
        topl_left.x = clipper->size.width - text_width + clipper->top_left.x;
        topl_left.y = clipper->top_left.y;
        break;
    case ei_anc_east:
        topl_left.x = clipper->size.width - text_width + clipper->top_left.x;
        topl_left.y = ((clipper->size.height - text_height) >> 1) + clipper->top_left.y;
        break;
    case ei_anc_southeast:
        topl_left.x = clipper->size.width - text_width + clipper->top_left.x;
        topl_left.y = clipper->size.height - text_height + clipper->top_left.y;
        break;
    case ei_anc_south:
        topl_left.x = ((clipper->size.width - text_width) >> 1) + clipper->top_left.x;
        topl_left.y = clipper->size.height - text_height + clipper->top_left.y;
        break;
    case ei_anc_southwest:
        topl_left.x = clipper->top_left.x;
        topl_left.y = clipper->size.height - text_height + clipper->top_left.y;
        break;
    case ei_anc_west:
        topl_left.x = clipper->top_left.x;
        topl_left.y = ((clipper->size.height - text_height) >> 1) + clipper->top_left.y;
        break;
    case ei_anc_northwest:
        topl_left.x = clipper->top_left.x;
        topl_left.y = clipper->top_left.y;
        break;
    default:
        break;
    }
    ei_draw_text(surface, &topl_left, ((ei_impl_frame_t *)widget)->text, ((ei_impl_frame_t *)widget)->text_font, ((ei_impl_frame_t *)widget)->text_color, clipper);
}

void ei_draw_frame_img(ei_surface_t surface, ei_widget_t widget, ei_rect_t *clipper)
{
    ei_rect_t src_rect;
    ei_rect_t dst_rect;
    if (((ei_impl_frame_t *)widget)->img_rect != NULL)
    {
        src_rect = *((ei_impl_frame_t *)widget)->img_rect;
    }
    else
    {
        src_rect = hw_surface_get_rect(((ei_impl_frame_t *)widget)->img);
    }
    int img_width = src_rect.size.width;
    int img_height = src_rect.size.height;

    if (img_width > clipper->size.width || img_height > clipper->size.height)
    {
        printf("img_rect too big to fit\n");
        return;
    }

    switch (((ei_impl_frame_t *)widget)->img_anchor)
    {
    case ei_anc_center:
        dst_rect.top_left.x = ((clipper->size.width - img_width) >> 1) + clipper->top_left.x;
        dst_rect.top_left.y = ((clipper->size.height - img_height) >> 1) + clipper->top_left.y;
        break;
    case ei_anc_north:
        dst_rect.top_left.x = ((clipper->size.width - img_width) >> 1) + clipper->top_left.x;
        dst_rect.top_left.y = clipper->top_left.y;
        break;
    case ei_anc_northeast:
        dst_rect.top_left.x = clipper->size.width - img_width + clipper->top_left.x;
        dst_rect.top_left.y = clipper->top_left.y;
        break;
    case ei_anc_east:
        dst_rect.top_left.x = clipper->size.width - img_width + clipper->top_left.x;
        dst_rect.top_left.y = ((clipper->size.height - img_height) >> 1) + clipper->top_left.y;
        break;
    case ei_anc_southeast:
        dst_rect.top_left.x = clipper->size.width - img_width + clipper->top_left.x;
        dst_rect.top_left.y = clipper->size.height - img_height + clipper->top_left.y;
        break;
    case ei_anc_south:
        dst_rect.top_left.x = ((clipper->size.width - img_width) >> 1) + clipper->top_left.x;
        dst_rect.top_left.y = clipper->size.height - img_height + clipper->top_left.y;
        break;
    case ei_anc_southwest:
        dst_rect.top_left.x = clipper->top_left.x;
        dst_rect.top_left.y = clipper->size.height - img_height + clipper->top_left.y;
        break;
    case ei_anc_west:
        dst_rect.top_left.x = clipper->top_left.x;
        dst_rect.top_left.y = ((clipper->size.height - img_height) >> 1) + clipper->top_left.y;
        break;
    case ei_anc_northwest:
        dst_rect.top_left.x = clipper->top_left.x;
        dst_rect.top_left.y = clipper->top_left.y;
        break;
    default:
        break;
    }
    dst_rect.size = src_rect.size;
    ei_copy_surface(surface, &dst_rect, ((ei_impl_frame_t *)widget)->img, &src_rect, true);
}

ei_widget_t ei_toplevel_allocfunc()
{
    ei_widget_t toplevel = (ei_widget_t)calloc(1, sizeof(ei_impl_toplevel_t));
    return toplevel;
}

void ei_toplevel_releasefunc(ei_widget_t toplevel)
{
    if (toplevel->placer_params != NULL)
    {
        free(toplevel->placer_params);
    }
    free(((ei_impl_toplevel_t *)toplevel)->min_size);
    free(((ei_impl_toplevel_t *)toplevel)->title);
}

void ei_toplevel_drawfunc(ei_widget_t toplevel,
                          ei_surface_t surface,
                          ei_surface_t pick_surface,
                          ei_rect_t *clipper)
{
    if (toplevel->placer_params != NULL)
    {
        ei_impl_placer_run(toplevel);

        /* content rectangle */
        ei_fill(surface, &((ei_impl_toplevel_t *)toplevel)->color, &toplevel->content_rect);

        /* bottom part of the border */
        ei_point_t bottom_border[11];
        bottom_border[0] = toplevel->content_rect.top_left;
        bottom_border[1].x = toplevel->content_rect.top_left.x;
        bottom_border[1].y = toplevel->content_rect.top_left.y + toplevel->content_rect.size.height;
        bottom_border[2].x = toplevel->content_rect.top_left.x + toplevel->content_rect.size.width - 15 + ((ei_impl_toplevel_t *)toplevel)->border_width;
        bottom_border[2].y = toplevel->content_rect.top_left.y + toplevel->content_rect.size.height;
        bottom_border[3].x = bottom_border[2].x;
        bottom_border[3].y = bottom_border[2].y - 15 + ((ei_impl_toplevel_t *)toplevel)->border_width;
        bottom_border[4].x = toplevel->content_rect.top_left.x + toplevel->content_rect.size.width;
        bottom_border[4].y = bottom_border[3].y;
        bottom_border[5].x = bottom_border[4].x;
        bottom_border[5].y = toplevel->content_rect.top_left.y;
        bottom_border[6].x = bottom_border[5].x + ((ei_impl_toplevel_t *)toplevel)->border_width;
        bottom_border[6].y = bottom_border[5].y;
        bottom_border[7].x = bottom_border[6].x;
        bottom_border[7].y = bottom_border[5].y + toplevel->content_rect.size.height + ((ei_impl_toplevel_t *)toplevel)->border_width;
        bottom_border[8].x = toplevel->screen_location.top_left.x;
        bottom_border[8].y = bottom_border[5].y + toplevel->content_rect.size.height + ((ei_impl_toplevel_t *)toplevel)->border_width;
        bottom_border[9].x = toplevel->screen_location.top_left.x;
        bottom_border[9].y = toplevel->content_rect.top_left.y;
        bottom_border[10] = bottom_border[0];

        ei_color_t border_color = {100, 100, 100, 255};
        ei_draw_polygon(surface, bottom_border, 11, border_color, clipper);

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

        ei_draw_polygon(surface, upper_border, 2 * length + 7, border_color, clipper);

        /* close button */
        ei_rect_t button_square;
        button_square.top_left.x = close_button_center.x - close_button_radius;
        button_square.top_left.y = close_button_center.y - close_button_radius;
        button_square.size.width = close_button_radius << 1;
        button_square.size.height = close_button_radius << 1;
        ei_draw_button(surface, button_square, (ei_color_t){200, 0, 0, 255}, ei_relief_raised, 2, close_button_radius);

        /* title */
        ei_point_t title_top_left = {toplevel->screen_location.top_left.x + (window_corner_radius << 1), toplevel->screen_location.top_left.y + ((ei_impl_toplevel_t *)toplevel)->border_width};
        ei_draw_text(surface, &title_top_left, ((ei_impl_toplevel_t *)toplevel)->title, ei_default_font, (ei_color_t){255, 255, 255, 255}, clipper);
        ei_widget_t child = toplevel->children_head;
        while (child != NULL)
        {
            (*(child->wclass->drawfunc))(child, surface, pick_surface, &child->screen_location);
            child = child->next_sibling;
        }
    }
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

void ei_toplevel_widgetclass_create(ei_widgetclass_t *ei_toplevel_widgetclass)
{
    ei_toplevel_widgetclass->allocfunc = &ei_toplevel_allocfunc;
    ei_toplevel_widgetclass->releasefunc = &ei_toplevel_releasefunc;
    ei_toplevel_widgetclass->drawfunc = &ei_toplevel_drawfunc;
    ei_toplevel_widgetclass->setdefaultsfunc = &ei_toplevel_setdefaultsfunc;
}