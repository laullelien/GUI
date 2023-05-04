#include <stdlib.h>

#include "ei_types.h"
#include "ei_event.h"
#include "ei_utils.h"
#include "hw_interface.h"
#include "ei_draw.h"

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

	ei_color_t *white = malloc(sizeof(ei_color_t));
	white->red = 0;
	white->green = 0;
	white->blue = 0;
	white->alpha = 0;

	ei_color_t *purple = malloc(sizeof(ei_color_t));
	purple->red = 255;
	purple->green = 0;
	purple->blue = 255;
	purple->alpha = 0;

	ei_color_t *yellow = malloc(sizeof(ei_color_t));
	yellow->red = 255;
	yellow->green = 255;
	yellow->blue = 0;
	yellow->alpha = 0;

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
	line_8[0].x = 550;
	line_8[0].y = 275;
	line_8[1].x = 500;
	line_8[1].y = 200;

	ei_fill(main_window, white, NULL);

	ei_draw_polyline(main_window, hexa_l, 9, *purple, NULL);
	ei_draw_polyline(main_window, pxl_p, 2, *purple, NULL);
	ei_draw_polyline(main_window, hexa_h, 9, *yellow, NULL);
	ei_draw_polyline(main_window, pxl_y, 2, *yellow, NULL);
	ei_draw_polyline(main_window, line_8, 2, *white, NULL);

	// unlock, update screen.
	hw_surface_unlock(main_window);
	hw_surface_update_rects(main_window, NULL);

	// Wait for a key press.
	event.type = ei_ev_none;
	while ((event.type != ei_ev_close) && (event.type != ei_ev_keydown))
		hw_event_wait_next(&event);

	// Free hardware resources.
	hw_quit();
	free(white);
	free(purple);
	free(yellow);
	// Terminate program with no error code.
	return 0;
}
