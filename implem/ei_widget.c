#include "../api/ei_widget.h"
#include "../api/ei_application.h"
#include "ei_implementation.h"
#include "ei_impl_widget.h"
#include "ei_widget_attributes.h"
#include <stdio.h>
#include "ei_impl_widgetclass.h"


static uint32_t current_picking_id;
static ei_widget_t * widget_list;


void initialize_widget_list()
{
    current_picking_id = 0;
    widget_list = calloc(10, sizeof(ei_widget_t));
}


int get_current_picking_id()
{
    return current_picking_id;
}

void insert_widget(ei_widget_t widget, ei_widget_t ** plist)
{
    if (current_picking_id % 10 == 9)
    {
        ei_widget_t * temp = calloc(current_picking_id + 11, sizeof(ei_widget_t));
        for (uint32_t i=0;i<current_picking_id;i++)
        {
            temp[i] = *plist[i];
        }
        *plist = temp;
    }
    else
    {
        *plist[current_picking_id+1] = widget;
    }
    current_picking_id++;
}



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
    child->pick_id = current_picking_id;
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
    // else if (strcmp("toplevel", widget->wclass->name) == 0)
    // {
    //     free((ei_impl_toplevel_t *)widget);
    // }
    while(child!=NULL)
    {
        next_sibling=child->next_sibling;
        ei_widget_destroy(child);
        child=next_sibling;
    }
}