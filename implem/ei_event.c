#include "../api/ei_event.h"
#include "ei_types.h"
#include "ei_event.h"


static ei_default_handle_func_t p_default_function = NULL;
static ei_widget_t active_widget;

void ei_event_set_default_handle_func(ei_default_handle_func_t func)
{
    // if (p_default_function != NULL)
    // {
    //     free(p_default_function);
    // }
    p_default_function = func;
    // *p_default_function = *func;
    // remember to free this function at the end
}


ei_default_handle_func_t	ei_event_get_default_handle_func(void)
{
    return p_default_function;
}


// ei_default_handle_func_t ei_event_get_default_handle_func()
// {

//     return
// }


void ei_event_set_active_widget(ei_widget_t widget)
{
    active_widget = widget;
}

ei_widget_t ei_event_get_active_widget()
{
    return active_widget;
}