#include "../api/ei_widget.h"
#include "../api/ei_application.h"
#include "ei_implementation.h"

ei_widget_t ei_widget_create(ei_const_string_t class_name,
                             ei_widget_t parent,
                             ei_user_param_t user_data,
                             ei_widget_destructor_t destructor)
{
    ei_widgetclass_t *wclass = ei_widgetclass_from_name(class_name);
    ei_widget_t widget = (*(wclass->allocfunc))();
    (*(wclass->setdefaultsfunc))(widget);
    (*(ei_widget_t *)widget) = malloc(sizeof(ei_impl_widget_t));
    (*(ei_widget_t *)widget)->parent = parent;
    (*(ei_widget_t *)widget)->user_data = user_data;
    // widget->destructor = ;
    return widget;
}