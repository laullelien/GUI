#include <stdio.h>
#include "../api/ei_widget.h"
#include "../api/ei_application.h"
#include "ei_implementation.h"
#include "ei_impl_widget.h"
#include "ei_widget_attributes.h"
#include "ei_impl_widgetclass.h"
#include "ei_impl_widget.h"
#include "ei_impl_event.h"
#include "ei_application.c"


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
    /* We remove the widget from the list of its parent children */
    ei_app_invalidate_rect(&widget->screen_location);
    if (widget->parent != NULL)
    {
        ei_widget_t p_widget = widget->parent->children_head;
        if (p_widget == widget)
        {
            widget->parent->children_head = widget->parent->children_head->next_sibling;
        }
        else
        {
            while (p_widget->next_sibling != widget)
            {
                p_widget = p_widget->next_sibling;
            }
            p_widget->next_sibling = widget->next_sibling;
            if (widget->parent->children_tail == widget)
            {
                widget->parent->children_tail = p_widget;
            }
        }
    }
    ei_widget_destroy_rec(widget);
}

bool ei_widget_is_displayed(ei_widget_t widget)
{
    return true;
}

ei_widget_t ei_widget_pick(ei_point_t *where)
{
    ei_surface_t pick_surface = ei_get_picking_surface();
    hw_surface_lock(pick_surface);
    uint32_t *p_first_pixel = (uint32_t *)hw_surface_get_buffer(pick_surface);
    p_first_pixel += where->x + where->y * hw_surface_get_size(pick_surface).width;
    hw_surface_unlock(pick_surface);
    return *(*get_widget_list_pointer() + ei_get_id(pick_surface, p_first_pixel));
}
