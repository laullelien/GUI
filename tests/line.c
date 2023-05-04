#include <stdlib.h>

#include "ei_types.h"
#include "ei_event.h"
#include "ei_utils.h"
#include "hw_interface.h"
#include "ei_draw.c"

int main(int argc, char* argv[])
{
	ei_surface_t			main_window		= NULL;
	ei_size_t			main_window_size	= ei_size(640, 480);
	ei_event_t			event;

	// Init acces to hardware.
	hw_init();

	// Create the main window.
	main_window = hw_create_window(main_window_size, false);

	// Lock the surface for drawing,
	hw_surface_lock(main_window);


	ei_color_t * fill_color = malloc(sizeof(ei_color_t));
	fill_color->red = 100;
	fill_color->green = 100;
	fill_color->blue = 100;
	fill_color->alpha = 100;

	ei_fill(main_window, fill_color, NULL);

    ei_point_t first_point;
    first_point.x = 100;
    first_point.y = 100;

    ei_point_t last_point;
    last_point.x = 400;
    last_point.y = 400;

    ei_draw_line(main_window, &first_point, &last_point, fill_color);

	// unlock, update screen.
	hw_surface_unlock(main_window);
	hw_surface_update_rects(main_window, NULL);

	// Wait for a key press.
	event.type = ei_ev_none;
	while ((event.type != ei_ev_close) && (event.type != ei_ev_keydown))
		hw_event_wait_next(&event);

	// Free hardware resources.
	hw_quit();
	free(fill_color);
	// Terminate program with no error code.
	return 0;
}
