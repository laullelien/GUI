#include "../api/ei_application.h"
#include "../api/ei_widgetclass.h"
#include "ei_impl_widgetclass.h"
#include "../api/hw_interface.h"
#include "../api/ei_widget_configure.h"
#include "ei_impl_widget.h"
#include "ei_types.h"
#include "ei_event.h"
#include "ei_impl_event.h"

static ei_widget_t root_widget;
static ei_widgetclass_t p_widgetclass_list[3];
static ei_surface_t root_surface;
static ei_surface_t pick_surface;
static ei_event_t event;
static bool stop = false;

ei_surface_t ei_get_picking_surface() {
    return pick_surface;
}

void ei_app_create(ei_size_t main_window_size, bool fullscreen)
{
    initialize_widget_list();
    /* initializes the hardware */
    hw_init();
    /* registers all classes of widget */
    strncpy(p_widgetclass_list->name, "frame", 6 * sizeof(char));
    ei_widgetclass_register(p_widgetclass_list);
    p_widgetclass_list->next = p_widgetclass_list + 1;
    strncpy(p_widgetclass_list->next->name, "button", 7 * sizeof(char));
    ei_widgetclass_register(p_widgetclass_list + 1);
    p_widgetclass_list->next->next = p_widgetclass_list + 2;
    strncpy(p_widgetclass_list->next->next->name, "toplevel", 9 * sizeof(char));
    ei_widgetclass_register(p_widgetclass_list + 2);

    /* creates the root window */
    root_surface = hw_create_window(main_window_size, fullscreen);
    pick_surface = hw_surface_create(root_surface, main_window_size, false);

    /* creates the root widget to access the root window. */
    root_widget = p_widgetclass_list->allocfunc();
    root_widget->wclass = p_widgetclass_list;
    root_widget->content_rect.size = main_window_size;
    root_widget->placer_params = calloc(1, sizeof(ei_impl_placer_params_t));
    ei_frame_setdefaultsfunc(root_widget);
    insert_widget(root_widget, get_widget_list_pointer());
}

void ei_app_run()
{
    hw_surface_lock(root_surface);
    hw_surface_lock(pick_surface);
    ei_rect_t root_rect = hw_surface_get_rect(root_surface);
    ei_frame_drawfunc(root_widget, root_surface, pick_surface, &root_rect);
    hw_surface_unlock(root_surface);
    hw_surface_unlock(pick_surface);
    hw_surface_update_rects(root_surface, NULL);
    free_p_rect_cell();
    ei_linked_rect_t *p_rect_cell;
    ei_widget_t active_widget;
    bool handled_event = false;

    while (!stop)
    {
        hw_surface_lock(root_surface);
        hw_surface_lock(pick_surface);
        hw_event_wait_next(&event);
        active_widget = ei_event_get_active_widget();
        handled_event = false;
        if ((event.type == ei_ev_close) && (event.type == ei_ev_keydown))
        {
            stop = true;
        }
        if (active_widget != NULL)
        {
            handled_event = (*(active_widget->wclass->handlefunc))(active_widget, &event);
        }
        /* if the event is localised */
        else if (event.type == ei_ev_mouse_move || event.type == ei_ev_mouse_buttondown || event.type == ei_ev_mouse_buttonup)
        {
            ei_widget_t clicked_widget = get_widget_from_mouse_location(&event, pick_surface);
            if (clicked_widget->wclass->handlefunc != NULL)
            {
                handled_event = (*(clicked_widget->wclass->handlefunc))(clicked_widget, &event);
            }
        }

        if (!handled_event && ei_event_get_default_handle_func() != NULL)
        {
            (*(ei_event_get_default_handle_func()))(&event);
        }

        p_rect_cell = get_p_rect_cell();
        merge_rect_clipper(p_rect_cell);
        while (p_rect_cell != NULL)
        {
            ei_frame_drawfunc(root_widget, root_surface, pick_surface, &(p_rect_cell->rect));
            p_rect_cell = p_rect_cell->next;
        }
        hw_surface_unlock(root_surface);
        hw_surface_unlock(pick_surface);
        if (get_p_rect_cell() != NULL)
        {
            hw_surface_update_rects(root_surface, NULL);
        }
        free_p_rect_cell();
    }
}

void ei_app_free()
{
    ei_widget_destroy(root_widget);
    hw_surface_free(pick_surface);
    hw_quit();
    free_p_rect_cell();
}

ei_widget_t ei_app_root_widget()
{
    return root_widget;
}

ei_surface_t ei_app_root_surface()
{
    return root_surface;
}

void ei_app_quit_request()
{
    stop = true;
}

void ei_app_invalidate_rect(const ei_rect_t *rect)
{
    if(rect==NULL)
    {
        return;
    }
    ei_linked_rect_t **pp_rect_cell = get_pp_rect_cell();
    if ((*pp_rect_cell) == NULL)
    {
        *pp_rect_cell = calloc(1, sizeof(ei_linked_rect_t));
        (*pp_rect_cell)->rect = *rect;
    }
    else
    {
        ei_linked_rect_t *p_rect_cell = get_p_rect_cell();
        while (p_rect_cell->next != NULL)
        {
            p_rect_cell = p_rect_cell->next;
        }
        p_rect_cell->next = calloc(1, sizeof(ei_linked_rect_t));
        p_rect_cell->next->rect = *rect;
    }
}



