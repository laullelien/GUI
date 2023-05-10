#include "../api/ei_widget.h"

ei_widget_t ei_widget_create(ei_const_string_t class_name,
                             ei_widget_t parent,
                             ei_user_param_t user_data,
                             ei_widget_destructor_t destructor)
{
    ei_widget_t *widget = malloc(sizeof(ei_widget_t));
    // widget->wclass->name = class_name;
    // widget->parent = parent;
    // widget->user_data = user_data;
    // widget->destructor = destructor;
    return *widget;
}