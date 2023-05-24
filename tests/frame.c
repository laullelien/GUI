// #include <stdio.h>
// #include <stdlib.h>

// #include "ei_application.h"
// #include "ei_event.h"
// #include "hw_interface.h"
// #include "ei_widget_configure.h"

// /*
//  * ei_main --
//  *
//  *	Main function of the application.
//  */
// int main(int argc, char** argv)
// {
// 	ei_widget_t	frame;

// 	/* Create the application and change the color of the background. */
// 	ei_app_create((ei_size_t){600, 600}, false);
// 	ei_frame_set_bg_color(ei_app_root_widget(), (ei_color_t){0x52, 0x7f, 0xb4, 0xff});

// 	/* Create, configure and place the frame on screen. */
// 	frame = ei_widget_create	("frame", ei_app_root_widget(), NULL, NULL);
// 	ei_frame_configure		(frame, &(ei_size_t){300,200},
// 			   			&(ei_color_t){0x88, 0x88, 0x88, 0xff},
// 			 			&(int){6},
// 					 	&(ei_relief_t){ei_relief_raised}, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
// 	ei_place_xy			(frame, 150, 200);

// 	/* Run the application's main loop. */
// 	ei_app_run();

// 	/* We just exited from the main loop. Terminate the application (cleanup). */
// 	ei_app_free();

// 	return (EXIT_SUCCESS);
// }

#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget_configure.h"

#include "../implem/ei_implementation.h"


#include "ei_draw.h"

/*
 * ei_main --
 *
 *	Main function of the application.
 */
int main(int argc, char **argv)
{
	ei_widget_t frame;
	static ei_string_t image_filename = "misc/klimt.jpg";
	ei_anchor_t anchor = ei_anc_center;
	ei_anchor_t img_anchor = ei_anc_west;
	ei_rect_t *src_rect=malloc(sizeof(ei_rect_t));
	src_rect->top_left.x=0;
	src_rect->top_left.y=0;
	src_rect->size.width=600;
	src_rect->size.height=400;
	
	/* Create the application and change the color of the background. */
	ei_app_create((ei_size_t){600, 600}, false);
	ei_frame_set_bg_color(ei_app_root_widget(), (ei_color_t){0x52, 0x7f, 0xb4, 0xff});
	ei_surface_t image = hw_image_load(image_filename, ei_app_root_surface());
	hw_surface_lock(image);
	/* Create, configure and place the frame on screen. */
	frame = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);

	ei_frame_configure(frame, &(ei_size_t){300, 200},
					   &(ei_color_t){0x88, 0x88, 0x88, 0xff},
					   &(int){2},
					   &(ei_relief_t){ei_relief_raised}, NULL, NULL, NULL, NULL, &image, &src_rect, &img_anchor);
	ei_place_xy(frame, 300, 300);
	ei_place(frame, &anchor, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	// hw_surface_free(image);
	/* Run the application's main loop. */
	ei_app_run();
	
	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}