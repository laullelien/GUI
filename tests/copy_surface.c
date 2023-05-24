#include <stdlib.h>
#include "ei_types.h"
#include "ei_event.h"
#include "ei_utils.h"
#include "hw_interface.h"
#include "ei_draw.h"
#include "../implem/ei_implementation.h"
#include "../implem/ei_draw_arc.h"

int main(int argc, char* argv[])
{
	ei_surface_t			main_window		= NULL;
	ei_size_t			main_window_size	= ei_size(640, 480);
	ei_event_t			event;

	// Init acces to hardware.
	hw_init();

	// Create the main window.
	main_window = hw_create_window(main_window_size, false);
	ei_surface_t copy_surface =hw_surface_create(main_window, main_window_size, true);

	// Lock the surface for drawing,
	hw_surface_lock(main_window);
	hw_surface_lock(copy_surface);

	ei_color_t white;
	white.red = 255;
	white.green = 255;
	white.blue = 255;
	white.alpha = 0;

	// fill white color on main window
    ei_fill(main_window, &white, NULL );
	
	
	ei_color_t copy_color;
	copy_color.red = 255;
	copy_color.green = 0;
	copy_color.blue = 0;
	copy_color.alpha = 60;
	
	// fill red color on second surface (copy_surface)
	ei_fill(copy_surface, &copy_color, NULL);


	ei_rect_t dst_rect;
	dst_rect.top_left.x=300;
	dst_rect.top_left.y=150;
	dst_rect.size.width=100;
	dst_rect.size.height=100;

	ei_rect_t src_rect;
	src_rect.top_left.x=200;
	src_rect.top_left.y=200;
	src_rect.size.width=100;
	src_rect.size.height=100;
	
	ei_copy_surface(main_window, &dst_rect, copy_surface, &src_rect, true);

	// unlock, update screen.
	hw_surface_unlock(main_window);
	hw_surface_unlock(copy_surface);
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