#include "../api/ei_widget.h"
#include "../api/ei_application.h"
#include "ei_implementation.h"
#include "ei_impl_widget.h"
#include "ei_widget_attributes.h"
#include <stdio.h>
#include "ei_impl_widgetclass.h"




ei_widget_t ei_widget_create(ei_const_string_t class_name,
                             ei_widget_t parent,
                             ei_user_param_t user_data,
                             ei_widget_destructor_t destructor)
{
    ei_widgetclass_t *wclass = ei_widgetclass_from_name(class_name);
    ei_widget_t child = (*(wclass->allocfunc))();
    child->wclass = wclass;
    (*(wclass->setdefaultsfunc))(child);
    child->parent = parent;
    ei_insert_child(parent, child);
    child->user_data = user_data;
    child->destructor = destructor;
    insert_widget(child, get_widget_list_pointer());
    return child;
}



void ei_widget_destroy(ei_widget_t widget)
{
    ei_placer_forget(widget);
    if (widget->wclass->releasefunc!=NULL)
    {
        (*(widget->wclass->releasefunc))(widget);
    }
    if(widget->destructor!=NULL)
    {
        (*(widget->destructor))(widget);
    }
    ei_widget_t child = widget->children_head;
    ei_widget_t next_sibling;
    if (strcmp("frame", widget->wclass->name) == 0)
    {
        free((ei_impl_frame_t *)widget);
    }
    else if (strcmp("button", widget->wclass->name) == 0)
    {
        free((ei_impl_button_t *)widget);
    }
    else if (strcmp("toplevel", widget->wclass->name) == 0)
    {
        free((ei_impl_toplevel_t *)widget);
    }
    while(child!=NULL)
    {
        next_sibling=child->next_sibling;
        ei_widget_destroy(child);
        child=next_sibling;
    }
}