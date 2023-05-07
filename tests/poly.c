#include <stdlib.h>

#include "ei_types.h"
#include "ei_event.h"
#include "ei_utils.h"
#include "hw_interface.h"
#include "ei_draw.h"

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

	ei_color_t *black = malloc(sizeof(ei_color_t));
	black->red = 0;
	black->green = 0;
	black->blue = 0;
	black->alpha = 0;

	ei_color_t *white = malloc(sizeof(ei_color_t));
	white->red = 255;
	white->green = 255;
	white->blue = 255;
	white->alpha = 0;

	ei_color_t *red = malloc(sizeof(ei_color_t));
	red->red = 255;
	red->green = 0;
	red->blue = 0;
	red->alpha = 0;

	ei_color_t *green = malloc(sizeof(ei_color_t));
	green->red = 0;
	green->green = 255;
	green->blue = 0;
	green->alpha = 0;

	ei_color_t *blue = malloc(sizeof(ei_color_t));
	blue->red = 0;
	blue->green = 0;
	blue->blue = 255;
	blue->alpha = 0;

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

	ei_point_t blue_poly[4];
	blue_poly[0].x = 8;
	blue_poly[0].y = 1;
	blue_poly[1].x = 13;
	blue_poly[1].y = 9;
	blue_poly[2].x = 11;
	blue_poly[2].y = 9;
	blue_poly[3].x = 8;
	blue_poly[3].y = 1;

	ei_point_t red_poly_b[5];
	red_poly_b[0].x = 570;
	red_poly_b[0].y = 120;
	red_poly_b[1].x = 610;
	red_poly_b[1].y = 120;
	red_poly_b[2].x = 670;
	red_poly_b[2].y = 280;
	red_poly_b[3].x = 570;
	red_poly_b[3].y = 280;
	red_poly_b[4].x = 570;
	red_poly_b[4].y = 120;

	ei_point_t green_poly_b[9];
	green_poly_b[0].x = 590;
	green_poly_b[0].y = 100;
	green_poly_b[1].x = 730;
	green_poly_b[1].y = 100;
	green_poly_b[2].x = 730;
	green_poly_b[2].y = 300;
	green_poly_b[3].x = 630;
	green_poly_b[3].y = 300;
	green_poly_b[4].x = 630;
	green_poly_b[4].y = 280;
	green_poly_b[5].x = 710;
	green_poly_b[5].y = 280;
	green_poly_b[6].x = 610;
	green_poly_b[6].y = 120;
	green_poly_b[7].x = 590;
	green_poly_b[7].y = 120;
	green_poly_b[8].x = 590;
	green_poly_b[8].y = 100;

	ei_point_t blue_poly_b[4];
	blue_poly_b[0].x = 610;
	blue_poly_b[0].y = 120;
	blue_poly_b[1].x = 710;
	blue_poly_b[1].y = 280;
	blue_poly_b[2].x = 670;
	blue_poly_b[2].y = 280;
	blue_poly_b[3].x = 610;
	blue_poly_b[3].y = 120;

	ei_point_t blue_poly_d[7];
	blue_poly_d[0].x = 9;
	blue_poly_d[0].y = 1;
	blue_poly_d[1].x = 13;
	blue_poly_d[1].y = 5;
	blue_poly_d[2].x = 12;
	blue_poly_d[2].y = 10;
	blue_poly_d[3].x = 7;
	blue_poly_d[3].y = 7;
	blue_poly_d[4].x = 2;
	blue_poly_d[4].y = 9;
	blue_poly_d[5].x = 2;
	blue_poly_d[5].y = 3;
	blue_poly_d[6].x = 9;
	blue_poly_d[6].y = 1;

	ei_point_t fish[7];
	fish[0].x = 200;
	fish[0].y = 50;
	fish[1].x = 350;
	fish[1].y = 100;
	fish[2].x = 100;
	fish[2].y = 500;
	fish[3].x = 200;
	fish[3].y = 450;
	fish[4].x = 300;
	fish[4].y = 500;
	fish[5].x = 50;
	fish[5].y = 100;
	fish[6].x = 200;
	fish[6].y = 50;

	ei_fill(main_window, black, NULL);

	ei_draw_polygon(main_window, red_poly, 5, *red, NULL);
	ei_draw_polygon(main_window, green_poly, 9, *green, NULL);
	ei_draw_polygon(main_window, blue_poly, 4, *blue, NULL);
	ei_draw_polygon(main_window, red_poly_b, 5, *red, NULL);
	ei_draw_polygon(main_window, green_poly_b, 9, *green, NULL);
	ei_draw_polygon(main_window, blue_poly_b, 4, *blue, NULL);
	ei_draw_polygon(main_window, fish, 7, *green, NULL);
	ei_draw_polyline(main_window, fish, 7, *white, NULL);
	ei_draw_polygon(main_window, blue_poly_d, 7, *blue, NULL);
	ei_draw_polyline(main_window, blue_poly_d, 7, *white, NULL);




	// unlock, update screen.
	hw_surface_unlock(main_window);
	hw_surface_update_rects(main_window, NULL);

	// Wait for a key press.
	event.type = ei_ev_none;
	while ((event.type != ei_ev_close) && (event.type != ei_ev_keydown))
		hw_event_wait_next(&event);

	// Free hardware resources.
	hw_quit();
	free(black);
	free(white);
	free(red);
	free(green);
	free(blue);
	// Terminate program with no error code.
	return 0;
}
