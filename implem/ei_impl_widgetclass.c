#include "../api/ei_widgetclass.h"
#include "ei_impl_widgetclass.h"
#include "ei_application.h"
#include "ei_draw_arc.h"

ei_widget_t ei_frame_allocfunc()
{
    ei_widget_t frame = (ei_widget_t)calloc(1, sizeof(ei_impl_frame_t));
    return frame;
}

void ei_frame_drawfunc(ei_widget_t frame, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    // if (((ei_impl_frame_t *)frame)->relief == ei_relief_none || ((ei_impl_frame_t *)frame)->border_width == 0)
    // {
    ei_fill(surface, &((ei_impl_frame_t *)frame)->color, clipper); //&frame->screen_location
    // ei_fill(pick_surface, frame->pick_color, &frame->screen_location);
    // }
    // else if (((ei_impl_frame_t *)frame)->relief == ei_relief_raised)
    // {
    //     // ei_draw_polygon(surface, );
    // }
    // else
    // {
    //     // ei_draw_text(surface, ((ei_impl_frame_t *)frame)->color);
    // }
    ei_widget_t children_head = frame->children_head;
    while (children_head != NULL)
    {
        ei_rect_t clipper;
        clipper.top_left = children_head->screen_location.top_left;
        clipper.size = children_head->requested_size;
        (*(children_head->wclass->drawfunc))(children_head, surface, pick_surface, &clipper);
        children_head = children_head->next_sibling;
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

/* BUTTON */

// ei_widget_t ei_button_allocfunc()
// {
//     ei_impl_widget_t* button = (ei_impl_widget_t*) malloc(sizeof(ei_button_t));
//     return button;
// }

// void ei_button_releasefunc(ei_widget_t button)
// {
//     free((ei_button_t *)button);
// }

// void ei_button_drawfunc(ei_widget_t button,
//                        ei_surface_t surface,
//                        ei_surface_t pick_surface,
//                        ei_rect_t *clipper)
// {
//     ei_draw_button(surface,rectangle);
// }

// void ei_button_setdefaultsfunc(ei_widget button)
// {
//     (ei_impl_button_t *)button->color = ei_default_background_color;
//     (ei_impl_button_t *)button->border_width = k_default_button_border_width;
//     (ei_impl_button_t *)button->corner_radius = k_default_button_corner_radius;
//     (ei_impl_button_t *)button->relief = ei_relief_raised;
//     (ei_impl_button_t *)button->text = NULL;
//     (ei_impl_button_t *)button->text_font = ei_default_font;
//     (ei_impl_button_t *)button->text_color = ei_font_default_color;
//     (ei_impl_button_t *)button->text_anchor = ei_anc_center;
//     (ei_impl_button_t *)button->img = NULL;
//     (ei_impl_button_t *)button->img_rect = NULL;
//     (ei_impl_button_t *)button->img_anchor = ei_anc_center;
//     (ei_impl_button_t *)button->callback = NULL;
//     (ei_impl_button_t *)button->user_param = NULL;
// }ei_widget_t ei_button_allocfunc()
// {
//     ei_impl_widget_t* button = (ei_impl_widget_t*) malloc(sizeof(ei_button_t));
//     return button;
// }

// void ei_button_releasefunc(ei_widget_t button)
// {
//     free((ei_button_t *)button);
// }

// void ei_button_drawfunc(ei_widget_t button,
//                        ei_surface_t surface,
//                        ei_surface_t pick_surface,
//                        ei_rect_t *clipper)
// {
//     ei_draw_button(surface,rectangle);
// }

// void ei_button_setdefaultsfunc(ei_widget button)
// {
//     (ei_impl_button_t *)button->color = ei_default_background_color;
//     (ei_impl_button_t *)button->border_width = k_default_button_border_width;
//     (ei_impl_button_t *)button->corner_radius = k_default_button_corner_radius;
//     (ei_impl_button_t *)button->relief = ei_relief_raised;
//     (ei_impl_button_t *)button->text = NULL;
//     (ei_impl_button_t *)button->text_font = ei_default_font;
//     (ei_impl_button_t *)button->text_color = ei_font_default_color;
//     (ei_impl_button_t *)button->text_anchor = ei_anc_center;
//     (ei_impl_button_t *)button->img = NULL;
//     (ei_impl_button_t *)button->img_rect = NULL;
//     (ei_impl_button_t *)button->img_anchor = ei_anc_center;
//     (ei_impl_button_t *)button->callback = NULL;
//     (ei_impl_button_t *)button->user_param = NULL;
// }