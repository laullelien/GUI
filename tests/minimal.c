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

    // create a second window
    ei_size_t second_window_size = ei_size(600,1000);
    ei_surface_t second_window = hw_surface_create(main_window, second_window_size, true);
    hw_surface_lock(second_window);

    // colors
    // ei_color_t black = {0,0,0,0};
    // ei_color_t white = {255,255,255,0};
    // ei_color_t red = {255,0,0,0};
    ei_color_t green = {0,255,0,80};
    ei_color_t blue = {0,0,255,200};

    // Test to copy surface
    ei_point_t dst_top_left = {350,350};
    ei_point_t src_top_left = {0,0};
    ei_size_t dst_size = {100,100};
    ei_size_t src_size = {100,100};
    ei_rect_t dst_rect = {dst_top_left, dst_size};
    ei_rect_t src_rect = {src_top_left, src_size};

    ei_fill(main_window, &blue, NULL);
    ei_fill(second_window, &green, NULL);
    ei_copy_surface(main_window, &dst_rect, second_window, &src_rect, true);


    // unlock, update screen.
    hw_surface_unlock(main_window);
    hw_surface_unlock(second_window);
    hw_surface_update_rects(main_window, NULL);

    // Wait for a key press.
    event.type = ei_ev_none;
    while ((event.type != ei_ev_close) && (event.type != ei_ev_keydown))
        hw_event_wait_next(&event);

    // Free hardware resources.
    hw_quit();
    // Terminate program with no error code.
    return 0;
}
