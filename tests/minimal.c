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

    // ei_color_t black;
    // black.green = 0;
    // black.red = 0;
    // black.blue = 0;
    // black.alpha = 0;

    ei_color_t white;
    white.red = 255;
    white.green = 255;
    white.blue = 255;
    white.alpha = 0;

    // ei_color_t red;
    // red.red = 255;
    // red.green = 0;
    // red.blue = 0;
    // red.alpha = 0;

    // ei_color_t green;
    // green.red = 0;
    // green.green = 255;
    // green.blue = 0;
    // green.alpha = 0;

    // ei_color_t blue;
    // blue.red = 0;
    // blue.green = 0;
    // blue.blue = 255;
    // blue.alpha = 0;

    // ei_point_t red_poly[5];
    // red_poly[0].x = 6;
    // red_poly[0].y = 1;
    // red_poly[1].x = 8;
    // red_poly[1].y = 1;
    // red_poly[2].x = 11;
    // red_poly[2].y = 9;
    // red_poly[3].x = 6;
    // red_poly[3].y = 9;
    // red_poly[4].x = 6;
    // red_poly[4].y = 1;

    // ei_point_t green_poly[9];
    // green_poly[0].x = 7;
    // green_poly[0].y = 0;
    // green_poly[1].x = 14;
    // green_poly[1].y = 0;
    // green_poly[2].x = 14;
    // green_poly[2].y = 10;
    // green_poly[3].x = 9;
    // green_poly[3].y = 10;
    // green_poly[4].x = 9;
    // green_poly[4].y = 9;
    // green_poly[5].x = 13;
    // green_poly[5].y = 9;
    // green_poly[6].x = 8;
    // green_poly[6].y = 1;
    // green_poly[7].x = 7;
    // green_poly[7].y = 1;
    // green_poly[8].x = 7;
    // green_poly[8].y = 0;

    // ei_point_t blue_poly[4];
    // blue_poly[0].x = 8;
    // blue_poly[0].y = 1;
    // blue_poly[1].x = 13;
    // blue_poly[1].y = 9;
    // blue_poly[2].x = 11;
    // blue_poly[2].y = 9;
    // blue_poly[3].x = 8;
    // blue_poly[3].y = 1;

    // ei_point_t red_poly_b[5];
    // red_poly_b[0].x = 570;
    // red_poly_b[0].y = 120;
    // red_poly_b[1].x = 610;
    // red_poly_b[1].y = 120;
    // red_poly_b[2].x = 670;
    // red_poly_b[2].y = 280;
    // red_poly_b[3].x = 570;
    // red_poly_b[3].y = 280;
    // red_poly_b[4].x = 570;
    // red_poly_b[4].y = 120;

    // ei_point_t green_poly_b[9];
    // green_poly_b[0].x = 590;
    // green_poly_b[0].y = 100;
    // green_poly_b[1].x = 730;
    // green_poly_b[1].y = 100;
    // green_poly_b[2].x = 730;
    // green_poly_b[2].y = 300;
    // green_poly_b[3].x = 630;
    // green_poly_b[3].y = 300;
    // green_poly_b[4].x = 630;
    // green_poly_b[4].y = 280;
    // green_poly_b[5].x = 710;
    // green_poly_b[5].y = 280;
    // green_poly_b[6].x = 610;
    // green_poly_b[6].y = 120;
    // green_poly_b[7].x = 590;
    // green_poly_b[7].y = 120;
    // green_poly_b[8].x = 590;
    // green_poly_b[8].y = 100;

    // ei_point_t blue_poly_b[4];
    // blue_poly_b[0].x = 610;
    // blue_poly_b[0].y = 120;
    // blue_poly_b[1].x = 710;
    // blue_poly_b[1].y = 280;
    // blue_poly_b[2].x = 670;
    // blue_poly_b[2].y = 280;
    // blue_poly_b[3].x = 610;
    // blue_poly_b[3].y = 120;

    // ei_point_t blue_poly_d[7];
    // blue_poly_d[0].x = 9;
    // blue_poly_d[0].y = 1;
    // blue_poly_d[1].x = 13;
    // blue_poly_d[1].y = 5;
    // blue_poly_d[2].x = 12;
    // blue_poly_d[2].y = 10;
    // blue_poly_d[3].x = 7;
    // blue_poly_d[3].y = 7;
    // blue_poly_d[4].x = 2;
    // blue_poly_d[4].y = 9;
    // blue_poly_d[5].x = 2;
    // blue_poly_d[5].y = 3;
    // blue_poly_d[6].x = 9;
    // blue_poly_d[6].y = 1;

    // ei_point_t fish[7];
    // fish[0].x = 200;
    // fish[0].y = 50;
    // fish[1].x = 350;
    // fish[1].y = 100;
    // fish[2].x = 100;
    // fish[2].y = 500;
    // fish[3].x = 200;
    // fish[3].y = 450;
    // fish[4].x = 300;
    // fish[4].y = 500;
    // fish[5].x = 50;
    // fish[5].y = 100;
    // fish[6].x = 200;
    // fish[6].y = 50;

    // ei_point_t hexa_l[9];
    // hexa_l[0].x = 100;
    // hexa_l[0].y = 100;
    // hexa_l[1].x = 200;
    // hexa_l[1].y = 100;
    // hexa_l[2].x = 250;
    // hexa_l[2].y = 125;
    // hexa_l[3].x = 250;
    // hexa_l[3].y = 175;
    // hexa_l[4].x = 200;
    // hexa_l[4].y = 200;
    // hexa_l[5].x = 100;
    // hexa_l[5].y = 200;
    // hexa_l[6].x = 50;
    // hexa_l[6].y = 175;
    // hexa_l[7].x = 50;
    // hexa_l[7].y = 125;
    // hexa_l[8].x = 100;
    // hexa_l[8].y = 100;

    // ei_point_t pxl_p[2];
    // pxl_p[0].x = 150;
    // pxl_p[0].y = 150;
    // pxl_p[1].x = 150;
    // pxl_p[1].y = 150;

    // ei_point_t hexa_h[9];
    // hexa_h[0].x = 400;
    // hexa_h[0].y = 100;
    // hexa_h[1].x = 350;
    // hexa_h[1].y = 100;
    // hexa_h[2].x = 300;
    // hexa_h[2].y = 175;
    // hexa_h[3].x = 300;
    // hexa_h[3].y = 250;
    // hexa_h[4].x = 350;
    // hexa_h[4].y = 325;
    // hexa_h[5].x = 400;
    // hexa_h[5].y = 325;
    // hexa_h[6].x = 450;
    // hexa_h[6].y = 250;
    // hexa_h[7].x = 450;
    // hexa_h[7].y = 175;
    // hexa_h[8].x = 400;
    // hexa_h[8].y = 100;

    // ei_point_t pxl_y[2];
    // pxl_y[0].x = 375;
    // pxl_y[0].y = 213;
    // pxl_y[1].x = 375;
    // pxl_y[1].y = 213;

    // ei_fill(main_window, black, NULL);

    // ei_draw_polyline(main_window, hexa_l, 9, *red, NULL);
    // ei_draw_polyline(main_window, pxl_p, 2, *red, NULL);
    // ei_draw_polyline(main_window, hexa_h, 9, *green, NULL);
    // ei_draw_polyline(main_window, pxl_y, 2, *green, NULL);

    // // ei_draw_polygon(main_window, red_poly, 5, *red, NULL);
    // // ei_draw_polygon(main_window, green_poly, 9, *green, NULL);
    // // ei_draw_polygon(main_window, blue_poly, 4, *blue, NULL);
    // // ei_draw_polygon(main_window, red_poly_b, 5, *red, NULL);
    // // ei_draw_polygon(main_window, green_poly_b, 9, *green, NULL);
    // // ei_draw_polygon(main_window, blue_poly_b, 4, *blue, NULL);
    // // ei_draw_polygon(main_window, fish, 7, *green, NULL);
    // // ei_draw_polyline(main_window, fish, 7, *white, NULL);
    // ei_draw_polygon(main_window, blue_poly_d, 7, *blue, NULL);
    // ei_draw_polyline(main_window, blue_poly_d, 7, *white, NULL);


    ei_fill(main_window, &white, NULL);




    // int radius = 120;
    // int length;
    ei_rect_t rectangle;
    rectangle.size.height = 200;
    rectangle.size.width = 500;
    rectangle.top_left.x = 200;
    rectangle.top_left.y = 200;
    // bool is_top1 = false;
    // bool is_top2 = true;
    // ei_point_t * list = list_of_points_for_a_rounded_frame(rectangle, radius,&length, is_top1);
    // ei_point_t * list2 = list_of_points_for_a_rounded_frame(rectangle, radius,&length, is_top2);

    // ei_draw_polygon(main_window, list, length, red, NULL);
    // ei_draw_polygon(main_window, list2, length, blue, NULL);

    ei_color_t color;
    color.red = 100;
    color.green = 0;
    color.blue = 100;
    color.alpha = 0;

    ei_draw_button(main_window, rectangle,color, true, 8, 20);

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
