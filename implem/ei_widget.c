#include "../api/ei_widget.h"
#include "../api/ei_application.h"
#include "ei_implementation.h"
#include "ei_impl_widget.h"
#include "ei_widget_attributes.h"

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
    return child;
}

void ei_insert_child(ei_widget_t parent, ei_widget_t child)
{
    if (parent->children_head == NULL)
    {
        parent->children_head = child;
        parent->children_tail = child;
    }
    else
    {
        parent->children_tail->next_sibling = child;
        parent->children_tail = child;
    }
}