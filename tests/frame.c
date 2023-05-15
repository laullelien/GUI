#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget_configure.h"

#include "../implem/ei_implementation.h"



/*
 * ei_main --
 *
 *	Main function of the application.
 */
int main(int argc, char** argv)
{
	ei_widget_t	frame;

	/* Create the application and change the color of the background. */
	ei_app_create((ei_size_t){600, 600}, false);
	ei_frame_set_bg_color(ei_app_root_widget(), (ei_color_t){0x52, 0x7f, 0xb4, 0xff});

	/* Create, configure and place the frame on screen. */
	frame = ei_widget_create	("frame", ei_app_root_widget(), NULL, NULL);
	ei_frame_configure		(frame, &(ei_size_t){300,200},
			   			&(ei_color_t){0x88, 0x88, 0x88, 0xff},
			 			&(int){6},
					 	&(ei_relief_t){ei_relief_raised}, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	printf("req sizee  %i %i\n", frame->requested_size.height, frame->requested_size.width);					
	ei_place_xy			(frame, 150, 200);
	printf("%i, %i\n", frame->screen_location.top_left.x, frame->screen_location.top_left.y);
	printf(" ici %i, %i\n", frame->screen_location.size.width, frame->screen_location.size.height);








	/* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}
