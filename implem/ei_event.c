#include "../api/ei_event.h"

static ei_widget_t active_widget;
static ei_default_handle_func_t p_default_function = NULL;

void ei_event_set_default_handle_func(ei_default_handle_func_t func)
{
    p_default_function = func;
}

ei_default_handle_func_t ei_event_get_default_handle_func(void)
{
    return p_default_function;
}

void ei_event_set_active_widget(ei_widget_t widget)
{
    active_widget = widget;
}

ei_widget_t ei_event_get_active_widget()
{
    return active_widget;
}