#include <stdlib.h>

#include "ei_types.h"
#include "ei_event.h"
#include "ei_utils.h"
#include "hw_interface.h"
#include "ei_draw.h"

// int main(int argc, char* argv[])
// {
// 	ei_surface_t			main_window		= NULL;
// 	ei_size_t			main_window_size	= ei_size(640, 480);
// 	ei_event_t			event;
// 	uint32_t			white			= 0xffffffff;
// 	uint8_t 			red[4]			= { 255, 255, 255, 255 };
// 	uint32_t*			pixel_ptr;
// 	int				i, ir, ig, ib, ia;

// 	// Init acces to hardware.
// 	hw_init();

// 	// Create the main window.
// 	main_window = hw_create_window(main_window_size, false);

// 	// Compute the value of a red pixel for this surface.
// 	hw_surface_get_channel_indices(main_window, &ir, &ig, &ib, &ia);
// 	red[ir]	= 255;
// 	red[ig]	= red[ib] = 0;

// 	// Lock the surface for drawing,
// 	hw_surface_lock(main_window);

// 	// fill the top half in white, the bottom half in red (hurray Poland!)
// 	pixel_ptr = (uint32_t*)hw_surface_get_buffer(main_window);

// 	for (i = 0; i < (main_window_size.width * (main_window_size.height/2)); i++)
// 		*pixel_ptr++ = white;

// 	for (i = 0; i < (main_window_size.width * (main_window_size.height/2)); i++)
// 		*pixel_ptr++ = *((uint32_t*)red);

// 	// unlock, update screen.
// 	hw_surface_unlock(main_window);
// 	hw_surface_update_rects(main_window, NULL);

// 	// Wait for a key press.
// 	event.type = ei_ev_none;
// 	while ((event.type != ei_ev_close) && (event.type != ei_ev_keydown))
// 		hw_event_wait_next(&event);

// 	// Free hardware resources.
// 	hw_quit();

// 	// Terminate program with no error code.
// 	return 0;
// }

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

	ei_color_t *green = malloc(sizeof(ei_color_t));
	green->red = 255;
	green->green = 255;
	green->blue = 255;
	green->alpha = 0;

	ei_color_t *black = malloc(sizeof(ei_color_t));
	black->red = 0;
	black->green = 0;
	black->blue = 0;
	black->alpha = 0;

	ei_color_t *blue = malloc(sizeof(ei_color_t));
	blue->red = 255;
	blue->green = 0;
	blue->blue = 255;
	blue->alpha = 0;

	ei_color_t *red = malloc(sizeof(ei_color_t));
	red->red = 255;
	red->green = 0;
	red->blue = 0;
	red->alpha = 0;

	ei_point_t red_poly[5];
	red_poly[0].x = 6;
	red_poly[0].y = 1;
	red_poly[1].x = 8;
	red_poly[1].y = 1;
	red_poly[2].x = 11;
	red_poly[2].y = 9;
	red_poly[3].x = 6;
	red_poly[3].y = 9;
	red_poly[4].x = 6;
	red_poly[4].y = 1;

	ei_point_t blue_poly[4];
	blue_poly[0].x = 8;
	blue_poly[0].y = 1;
	blue_poly[1].x = 13;
	blue_poly[1].y = 9;
	blue_poly[2].x = 11;
	blue_poly[2].y = 9;
	blue_poly[3].x = 8;
	blue_poly[3].y = 1;

	ei_point_t green_poly[9];
	green_poly[0].x = 7;
	green_poly[0].y = 0;
	green_poly[1].x = 14;
	green_poly[1].y = 0;
	green_poly[2].x = 14;
	green_poly[2].y = 10;
	green_poly[3].x = 9;
	green_poly[3].y = 10;
	green_poly[4].x = 9;
	green_poly[4].y = 9;
	green_poly[5].x = 13;
	green_poly[5].y = 9;
	green_poly[6].x = 8;
	green_poly[6].y = 1;
	green_poly[7].x = 7;
	green_poly[7].y = 1;
	green_poly[8].x = 7;
	green_poly[8].y = 0;

	ei_fill(main_window, black, NULL);

	ei_draw_polygon(main_window, red_poly, 5, *red, NULL);
	ei_draw_polygon(main_window, green_poly, 9, *green, NULL);
	ei_draw_polygon(main_window, blue_poly, 4, *blue, NULL);

	// unlock, update screen.
	hw_surface_unlock(main_window);
	hw_surface_update_rects(main_window, NULL);

	// Wait for a key press.
	event.type = ei_ev_none;
	while ((event.type != ei_ev_close) && (event.type != ei_ev_keydown))
		hw_event_wait_next(&event);

	// Free hardware resources.
	hw_quit();
	free(green);
	free(black);
	free(blue);
	free(red);
	// Terminate program with no error code.
	return 0;
}
