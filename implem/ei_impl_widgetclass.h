#include "../api/ei_widgetclass.h"
#include "ei_implementation.h"
#include "ei_types.h"

typedef struct ei_impl_frame_t
{
    ei_impl_widget_t ei_frame_widget;
    ei_color_t color;
    int border_width;
    ei_relief_t relief;
    ei_string_t text;
    ei_font_t text_font;
    ei_color_t text_color;
    ei_anchor_t text_anchor;
    ei_surface_t img;
    ei_rect_ptr_t img_rect;
    ei_anchor_t img_anchor;
} ei_impl_frame_t;

ei_widget_t ei_frame_allocfunc();
void ei_frame_releasefunc(ei_widget_t frame);
void ei_frame_drawfunc(ei_widget_t frame,
                       ei_surface_t surface,
                       ei_surface_t pick_surface,
                       ei_rect_t *clipper);
void ei_frame_setdefaultsfunc(ei_widget_t frame);
void ei_frame_widgetclass_create(ei_widgetclass_t *ei_frame_widgetclass);
void ei_widgetclass_register(ei_widgetclass_t *widgetclass);
ei_widgetclass_t *ei_widgetclass_from_name(ei_const_string_t name);

/* BUTTON */
typedef struct ei_impl_button_t
{
    ei_impl_widget_t button_widget;
    ei_color_t color;
    int border_width;
    int corner_radius;
    ei_relief_t relief;
    ei_string_t text;
    ei_font_t text_font;
    ei_color_t text_color;
    ei_anchor_t text_anchor;
    ei_surface_t img;
    ei_rect_ptr_t img_rect;
    ei_anchor_t img_anchor;
    ei_callback_t callback;
    ei_user_param_t user_param;
} ei_impl_button_t;

ei_widget_t ei_button_allocfunc();
void ei_button_releasefunc(ei_widget_t button);
void ei_button_drawfunc(ei_widget_t frame,
                        ei_surface_t surface,
                        ei_surface_t pick_surface,
                        ei_rect_t *clipper);
void ei_button_setdefaultsfunc(ei_widget_t button);
void ei_button_widgetclass_create(ei_widgetclass_t *ei_frame_widgetclass);
