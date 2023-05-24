#include "../api/ei_widgetclass.h"
#include "ei_impl_widgetclass.h"
#include "ei_application.h"
#include "ei_draw_arc.h"

void ei_widgetclass_register(ei_widgetclass_t *p_widgetclass_list)
{
    if (strcmp("frame", p_widgetclass_list->name) == 0)
    {
        ei_frame_widgetclass_create(p_widgetclass_list);
    }
    else if (strcmp("button", p_widgetclass_list->name) == 0)
    {
        ei_button_widgetclass_create(p_widgetclass_list);
    }
    else if (strcmp("toplevel", p_widgetclass_list->name) == 0)
    {                                 
        ei_toplevel_widgetclass_create(p_widgetclass_list);
    }
    else
    return;
}

ei_widgetclass_t *ei_widgetclass_from_name(ei_const_string_t name)
{
    ei_widgetclass_t *p_widgetclass_list = ei_app_root_widget()->wclass;
    if (strcmp("frame", name) == 0)
    {
        return p_widgetclass_list;
    }
    else if (strcmp("button", name) == 0)
    {
        return p_widgetclass_list + 1;
    }
    else if (strcmp("toplevel", name) == 0)
    {
        return p_widgetclass_list + 2;
    }
    return NULL;
}


size_t		ei_widget_struct_size()
{
    return sizeof(ei_impl_widget_t);
}