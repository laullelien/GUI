#include "ei_types.h"
#include "ei_widgetclass.h"
#include "ei_event.h"
#include "ei_impl_widget.h"
#include "ei_impl_widgetclass.h"
#include "ei_impl_event.h"
#include "ei_implementation.h"


ei_widget_t get_widget_from_mouse_location(ei_event_t *event, ei_surface_t pick_surface)
{
    uint32_t *p_first_pixel = (uint32_t *)hw_surface_get_buffer(pick_surface);
    ei_point_t mouse_location = event->param.mouse.where;
    p_first_pixel += mouse_location.x + mouse_location.y * hw_surface_get_size(pick_surface).width;
    return *(*get_widget_list_pointer() + ei_get_red(pick_surface, p_first_pixel));
}

void merge_rect_clipper(ei_linked_rect_t * rects)
{
    int rect_sum_area = 0;
    ei_linked_rect_t * current = rects;

    int minimal_top = current->rect.top_left.y;;
    int minimal_left = current->rect.top_left.x;
    int maximal_right = current->rect.top_left.x + current->rect.size.width;
    int maximal_bottom = current->rect.top_left.y + current->rect.size.height;


    while (current != NULL)
    {
        int x_top_left = current->rect.top_left.x;
        int y_top_left = current->rect.top_left.y;
        int x_bottom_right = current->rect.top_left.x + current->rect.size.width;
        int y_bottom_right = current->rect.top_left.y + current->rect.size.height;


        if (minimal_top > y_top_left)
        {
            minimal_top = y_top_left;
        }

        if (minimal_left > x_top_left)
        {
            minimal_left = x_top_left;
        }

        if (maximal_bottom < y_bottom_right)
        {
            maximal_bottom = y_bottom_right;
        }

        if (maximal_right < x_bottom_right)
        {
            maximal_right = x_bottom_right;
        }


        rect_sum_area += (current->rect.size.height * current->rect.size.width);
        current = current->next;
    }

    ei_point_t main_top_left = {minimal_left, minimal_top};
    ei_point_t main_bottom_right = {maximal_right, maximal_bottom};

    int total_area = (main_bottom_right.x - main_top_left.x)*(main_bottom_right.y - main_top_left.y);

    if (total_area < rect_sum_area)
    {

        ei_rect_t rect;
        rect.top_left.x = minimal_left;
        rect.top_left.y = minimal_top;
        rect.size.width = main_bottom_right.x - main_top_left.x;
        rect.size.height = main_bottom_right.y - main_top_left.y;

        ei_linked_rect_t * current2 = rects->next;
        ei_linked_rect_t * last = rects->next;

        if (current2== NULL)
        {
            rects->rect = rect;
        }
        else
        {
            while (current2 != NULL)
            {
                current2 = current2->next;
                free(last);
                last = current2;
            }
            rects->next = NULL;
            rects->rect = rect;
        }
    }
}
