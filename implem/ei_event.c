#include "../api/ei_event.h"

// ei_event_set_default_handle_func(ei_default_handle_func_t func)
// {
// }

// ei_default_handle_func_t ei_event_get_default_handle_func()
// {

//     return
// }

static ei_widget_t active_widget;

void ei_event_set_active_widget(ei_widget_t widget)
{
    active_widget = widget;
}

ei_widget_t ei_event_get_active_widget()
{
    return active_widget;
}