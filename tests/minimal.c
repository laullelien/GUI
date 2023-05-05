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

	ei_color_t white = {255,255,255,0};

	ei_color_t black = {0,0,0,0};

	ei_color_t purple = {255,0,255,0};

	ei_color_t yellow = {255,255,0,0};


	ei_point_t hexa_l[9];
	hexa_l[0].x = 100;
	hexa_l[0].y = 100;
	hexa_l[1].x = 200;
	hexa_l[1].y = 100;
	hexa_l[2].x = 250;
	hexa_l[2].y = 125;
	hexa_l[3].x = 250;
	hexa_l[3].y = 175;
	hexa_l[4].x = 200;
	hexa_l[4].y = 200;
	hexa_l[5].x = 100;
	hexa_l[5].y = 200;
	hexa_l[6].x = 50;
	hexa_l[6].y = 175;
	hexa_l[7].x = 50;
	hexa_l[7].y = 125;
	hexa_l[8].x = 100;
	hexa_l[8].y = 100;

	ei_point_t pxl_p[2];
	pxl_p[0].x = 150;
	pxl_p[0].y = 150;
	pxl_p[1].x = 150;
	pxl_p[1].y = 150;

	ei_point_t hexa_h[9];
	hexa_h[0].x = 400;
	hexa_h[0].y = 100;
	hexa_h[1].x = 350;
	hexa_h[1].y = 100;
	hexa_h[2].x = 300;
	hexa_h[2].y = 175;
	hexa_h[3].x = 300;
	hexa_h[3].y = 250;
	hexa_h[4].x = 350;
	hexa_h[4].y = 325;
	hexa_h[5].x = 400;
	hexa_h[5].y = 325;
	hexa_h[6].x = 450;
	hexa_h[6].y = 250;
	hexa_h[7].x = 450;
	hexa_h[7].y = 175;
	hexa_h[8].x = 400;
	hexa_h[8].y = 100;

	ei_point_t pxl_y[2];
	pxl_y[0].x = 375;
	pxl_y[0].y = 213;
	pxl_y[1].x = 375;
	pxl_y[1].y = 213;

	ei_point_t line_8[2];
	line_8[0].x = 300;
	line_8[0].y = 275;
	line_8[1].x = 500;
	line_8[1].y = 200;

	ei_point_t top_left = {0, 0};
	ei_size_t size = {400,400};
	ei_rect_t clipper = {top_left, size};


	ei_fill(main_window, &black, NULL);
	ei_fill(main_window, &yellow, &clipper);



	ei_draw_polyline(main_window, hexa_l, 9, purple, NULL);
	ei_draw_polyline(main_window, pxl_p, 2, purple, NULL);
	ei_draw_polyline(main_window, hexa_h, 9, yellow, NULL);
	ei_draw_polyline(main_window, pxl_y, 2, yellow, NULL);
	ei_draw_polyline(main_window, line_8, 2, white, NULL);
	
	// unlock, update screen.
	hw_surface_unlock(main_window);
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

