#include <stdlib.h>
#include "ei_types.h"
#include "ei_event.h"
#include "ei_utils.h"
#include "hw_interface.h"
#include "ei_draw.h"
#include "../implem/ei_implementation.h"
#include "../implem/ei_draw_arc.h"

int main(int argc, char *argv[])
{
    ei_surface_t main_window = NULL;
    ei_size_t main_window_size = ei_size(800, 800);
    ei_event_t event;

    // Init acces to hardware.
    hw_init();

    // Create the main window.
    main_window = hw_create_window(main_window_size, false);

    // Lock the surface for drawing,
    hw_surface_lock(main_window);

    ei_color_t black = {0,0,0,0};
    ei_color_t white = {255,255,255,0};
    ei_color_t red = {255,0,0,0};
    ei_color_t green = {0,255,0,0};
    ei_color_t blue = {0,0,255,0};

    ei_size_t second_window_size = ei_size(1200,600);
    ei_surface_t second_window = hw_create_window(second_window_size, false);
    hw_surface_lock(second_window);

    ei_fill(main_window, red, NULL);
    ei_fill(second_window, yelow, NULL);

    // unlock, update screen.
    hw_surface_unlock(main_window);
    hw_surface_update_rects(main_window, NULL);
    hw_surface_lock(second_window);
    hw_surface_update_rects(second_window, NULL);

    // Wait for a key press.
    event.type = ei_ev_none;
    while ((event.type != ei_ev_close) && (event.type != ei_ev_keydown))
        hw_event_wait_next(&event);

    // Free hardware resources.
    hw_quit();
    // Terminate program with no error code.
    return 0;
}
