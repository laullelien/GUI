#include "ei_impl_widget.h"
#include "ei_implementation.h"

static uint8_t current_picking_id;
static ei_widget_t *widget_list;

void initialize_widget_list()
{
    current_picking_id = 0;
    widget_list = calloc(10, sizeof(ei_widget_t));
}
ei_widget_t **get_widget_list_pointer()
{
    return &widget_list;
}

void insert_widget(ei_widget_t widget, ei_widget_t **plist)
{

    if (current_picking_id % 10 == 9)
    {
        ei_widget_t *temp = calloc(current_picking_id + 11, sizeof(ei_widget_t));
        for (uint8_t i = 0; i < current_picking_id; i++)
        {
            temp[i] = *plist[i];
        }
        *plist = temp;
    }
    else
    {
        widget->pick_id = current_picking_id;
        (*plist)[current_picking_id] = widget;
    }
    current_picking_id++;
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