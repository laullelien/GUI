#include "../api/ei_widgetclass.h"
#include "ei_impl_widgetclass.h"
#include "ei_application.h"

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