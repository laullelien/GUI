#include "../api/ei_widgetclass.h"
#include "ei_impl_widgetclass.h"
#include "ei_draw.h"
#include "ei_application.h"

ei_widget_t ei_frame_allocfunc()
{
    ei_widget_t frame = (ei_widget_t)malloc(sizeof(ei_frame_t));
    return frame;
}

void ei_frame_releasefunc(ei_widget_t frame)
{
    free((ei_frame_t *)frame);
}

void ei_frame_drawfunc(ei_widget_t frame,
                       ei_surface_t surface,
                       ei_surface_t pick_surface,
                       ei_rect_t *clipper)
{
    if (((ei_frame_t *)frame)->relief == ei_relief_none || ((ei_frame_t *)frame)->border_width == 0)
    {
        ei_fill(surface, &((ei_frame_t *)frame)->color, &(*(ei_widget_t *)frame)->screen_location);
        ei_fill(pick_surface, ((ei_frame_t *)frame)->ei_frame_widget->pick_color, &(*(ei_widget_t *)frame)->screen_location);
    }
    else if (((ei_frame_t *)frame)->relief == ei_relief_raised)
    {
        // ei_draw_polygon(surface, );
    }

    // ei_draw_text(surface, ((ei_frame_t *)frame)->color);
    ei_widget_t children_head = ((ei_frame_t *)frame)->ei_frame_widget->children_head;
    while (children_head != NULL)
    {
        (*((*(ei_widget_t *)children_head)->wclass->drawfunc))(children_head, surface, pick_surface, (*(ei_widget_t *)frame)->content_rect);
        children_head = children_head->next_sibling;
    }
}

void ei_frame_setdefaultsfunc(ei_widget_t frame)
{
    // (*(ei_widget_t *)frame)->requested_size = *requested_size;
    ((ei_frame_t *)frame)->color = ei_default_background_color;
    ((ei_frame_t *)frame)->border_width = 0;
    ((ei_frame_t *)frame)->relief = ei_relief_none;
    ((ei_frame_t *)frame)->text = NULL;
    ((ei_frame_t *)frame)->text_font = ei_default_font;
    ((ei_frame_t *)frame)->text_color = ei_font_default_color;
    ((ei_frame_t *)frame)->text_anchor = ei_anc_center;
    ((ei_frame_t *)frame)->img = NULL;
    ((ei_frame_t *)frame)->img_rect = NULL;
    ((ei_frame_t *)frame)->img_anchor = ei_anc_center;
    printf("ici\n");
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