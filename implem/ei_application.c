#include "../api/ei_application.h"
#include "../api/ei_widgetclass.h"
#include "ei_impl_widgetclass.h"
#include "../api/hw_interface.h"
#include "../api/ei_widget_configure.h"

static ei_widget_t root_widget;
static ei_widgetclass_t widgetclass[1];
static ei_surface_t root_surface;
static ei_surface_t pick_surface;

void ei_app_create(ei_size_t main_window_size, bool fullscreen)
{
    /* initializes the hardware */
    hw_init();
    /* registers all classes of widget */
    ei_widgetclass_register(widgetclass);
    /* creates the root window */
    root_surface = hw_create_window(main_window_size, fullscreen);
    pick_surface = hw_surface_create(root_surface, main_window_size, false);
    /* creates the root widget to access the root window. */
    root_widget = widgetclass->allocfunc();
    root_widget->wclass = widgetclass;
    ei_frame_setdefaultsfunc(root_widget);
}

void ei_app_run()
{
    hw_surface_lock(root_surface);
    hw_surface_lock(pick_surface);
    ei_frame_drawfunc(root_widget, root_surface, pick_surface, NULL);
    hw_surface_unlock(root_surface);
    hw_surface_unlock(pick_surface);
    hw_surface_update_rects(root_surface, NULL);
    getchar();
}

void ei_app_free()
{
}

ei_widget_t ei_app_root_widget()
{
    return root_widget;
}

ei_surface_t ei_app_root_surface(void)
{
    return root_surface;
}