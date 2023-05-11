#include "../api/ei_widgetclass.h"
#include "ei_impl_widgetclass.h"
#include "ei_draw.h"
#include "ei_application.h"
#include 

ei_widget_t ei_frame_allocfunc()
{
    ei_widget_t frame = (ei_widget_t)calloc(1, sizeof(ei_impl_frame_t));
    return frame;
}

void ei_frame_releasefunc(ei_widget_t frame_widget)
{
    free((ei_impl_frame_t *)frame_widget);
}

void ei_frame_drawfunc(ei_widget_t frame,
                       ei_surface_t surface,
                       ei_surface_t pick_surface,
                       ei_rect_t *clipper)
{
    if (((ei_impl_frame_t *)frame)->relief == ei_relief_none || ((ei_impl_frame_t *)frame)->border_width == 0)
    {
        ei_color_t *cl = &((ei_impl_frame_t *)frame)->color;
        printf("2. %u, %p\n", *((uint32_t *)cl), cl);
        ei_fill(surface, &((ei_impl_frame_t *)frame)->color, &frame->screen_location);
        ei_fill(pick_surface, frame->pick_color, &frame->screen_location);
    }
    else if (((ei_impl_frame_t *)frame)->relief == ei_relief_raised)
    {
        // ei_draw_polygon(surface, );
    }

    // ei_draw_text(surface, ((ei_impl_frame_t *)frame)->color);
    ei_widget_t children_head = frame->children_head;
    while (children_head != NULL)
    {
        (*(children_head->wclass->drawfunc))(children_head, surface, pick_surface, frame->content_rect);
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
    ei_frame_widgetclass->name[0] = 'f';
    ei_frame_widgetclass->name[1] = 'r';
    ei_frame_widgetclass->name[2] = 'a';
    ei_frame_widgetclass->name[3] = 'm';
    ei_frame_widgetclass->name[4] = 'e';
    ei_frame_widgetclass->allocfunc = &ei_frame_allocfunc;
    ei_frame_widgetclass->releasefunc = &ei_frame_releasefunc;
    ei_frame_widgetclass->drawfunc = &ei_frame_drawfunc;
    ei_frame_widgetclass->setdefaultsfunc = &ei_frame_setdefaultsfunc;
}

void ei_widgetclass_register(ei_widgetclass_t *widgetclass)
{
    ei_frame_widgetclass_create(widgetclass);
    // widgetclass->next=ei_button_widgetclass_create(widgetclass);
    // widgetclass->next->next=ei_toplevel_widgetclass_create(widgetclass);
    return;
}

ei_widgetclass_t *ei_widgetclass_from_name(ei_const_string_t name)
{
    ei_widgetclass_t *widgetclass = ei_app_root_widget()->wclass;
    if (widgetclass->name[0] == 'f')
    {
        return widgetclass;
    }
    return NULL;
}


/* BUTTON */





ei_widget_t ei_button_allocfunc()
{
    ei_impl_widget_t* button = (ei_impl_widget_t*) malloc(sizeof(ei_button_t));
    return button;
}

void ei_button_releasefunc(ei_widget_t button)
{
    free((ei_button_t *)button);
}

void ei_button_drawfunc(ei_widget_t button,
                       ei_surface_t surface,
                       ei_surface_t pick_surface,
                       ei_rect_t *clipper)
{

}


void ei_button_setdefaultsfunc(ei_widget button)
{
    (ei_impl_button_t *)button->color = ei_default_background_color;
    (ei_impl_button_t *)button->border_width = k_default_button_border_width;
    (ei_impl_button_t *)button->corner_radius = k_default_button_corner_radius;
    (ei_impl_button_t *)button->relief = ei_relief_raised;
    (ei_impl_button_t *)button->text = NULL;
    (ei_impl_button_t *)button->text_font = ei_default_font;
    (ei_impl_button_t *)button->text_color = ei_font_default_color;
    (ei_impl_button_t *)button->text_anchor = ei_anc_center;
    (ei_impl_button_t *)button->img = NULL;
    (ei_impl_button_t *)button->img_rect = NULL;
    (ei_impl_button_t *)button->img_anchor = ei_anc_center;
    (ei_impl_button_t *)button->callback = NULL;
    (ei_impl_button_t *)button->user_param = NULL;
}


