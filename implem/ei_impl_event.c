#include "ei_types.h"
#include "ei_widgetclass.h"
#include "ei_event.h"
#include "ei_impl_widget.h"


ei_widget_t get_widget_from_mouse_location(ei_event_t *event, ei_surface_t pick_surface)
{
    uint32_t* p_first_pixel = (uint32_t*) hw_surface_get_buffer(pick_surface);
    ei_point_t mouse_location = event->param.mouse.where;
    p_first_pixel += mouse_location.x + mouse_location.y * hw_surface_get_size(pick_surface).width;
    return *(*get_widget_list_pointer() + *p_first_pixel);
}

