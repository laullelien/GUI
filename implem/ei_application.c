#include "../api/ei_application.h"
#include "../api/ei_widgetclass.h"
#include "ei_impl_widgetclass.h"
#include "../api/hw_interface.h"
#include "../api/ei_widget_configure.h"

static ei_widget_t root_widget;
static ei_widgetclass_t p_widgetclass_list[3];
static ei_surface_t root_surface;
static ei_surface_t pick_surface;

void ei_app_create(ei_size_t main_window_size, bool fullscreen)
{
    /* initializes the hardware */
    hw_init();
    /* registers all classes of widget */
    strncpy(p_widgetclass_list->name, "frame", 6 * sizeof(char));
    ei_widgetclass_register(p_widgetclass_list);
    p_widgetclass_list->next = p_widgetclass_list + 1;
    // strncpy(p_widgetclass_list->next->name, "button", 7 * sizeof(char));
    // ei_widgetclass_register(p_widgetclass_list+1);
    // p_widgetclass_list->next->next = p_widgetclass_list + 2;
    // strncpy(p_widgetclass_list->next->next->name, "toplevel", 9 * sizeof(char));
    // ei_widgetclass_register(p_widgetclass_list+2);
    /* creates the root window */
    root_surface = hw_create_window(main_window_size, fullscreen);
    pick_surface = hw_surface_create(root_surface, main_window_size, false);
    /* creates the root widget to access the root window. */
    root_widget = p_widgetclass_list->allocfunc();
    root_widget->wclass = p_widgetclass_list;
    root_widget->screen_location.size = main_window_size;
    root_widget->placer_params = calloc(1, sizeof(ei_impl_placer_params_t));
    ei_frame_setdefaultsfunc(root_widget);
}

void ei_app_run()
{
    hw_surface_lock(root_surface);
    // hw_surface_lock(pick_surface);
    ei_rect_t root_rect=hw_surface_get_rect(root_surface);
    ei_frame_drawfunc(root_widget, root_surface, pick_surface, &root_rect);
    hw_surface_unlock(root_surface);
    // hw_surface_unlock(pick_surface);
    hw_surface_update_rects(root_surface, NULL);
    getchar();
}

void ei_app_free()
{
    ei_widget_destroy(root_widget);
    hw_surface_free(pick_surface);
    hw_quit();
}

ei_widget_t ei_app_root_widget()
{
    return root_widget;
}

ei_surface_t ei_app_root_surface()
{
    return root_surface;
}