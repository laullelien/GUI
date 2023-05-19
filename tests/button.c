#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget_configure.h"
#include "../implem/ei_implementation.h"
#include "../implem/ei_impl_widget.h"

/*
 * button_press --
 *
 *	Callback called when a user clicks on the button.
 */
void button_press(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param)
{
	printf("Click !\n");
}

/*
 * process_key --
 *
 *	Callback called when any key is pressed by the user.
 *	Simply looks for the "Escape" key to request the application to quit.
 */
// bool process_key(ei_event_t* event)
// {
// 	if ( (event->type == ei_ev_close) ||
// 	    ((event->type == ei_ev_keydown) && (event->param.key.key_code == SDLK_ESCAPE))) {
// 		ei_app_quit_request();
// 		return true;
// 	} else
// 		return false;
// }

/*
 * ei_main --
 *
 *	Main function of the application.
 */
int main(int argc, char** argv)
{
	ei_widget_t	button;

	/* Create the application and change the color of the background. */
	ei_app_create			((ei_size_t){600, 600}, false);
	ei_frame_set_bg_color		(ei_app_root_widget(), (ei_color_t){0x52, 0x7f, 0xb4, 0xff});
	// ei_event_set_default_handle_func(process_key);


	/* Create, configure and place the button on screen. */

    // ei_rect_t * rect = malloc(sizeof(ei_rect_t));
    // rect->top_left.y=50;
    // rect->size.width=3000;
    // rect->top_left.x=50;
    // rect->size.height=3350;


    // static ei_string_t image_filename = "misc/klimt.jpg";
	button = ei_widget_create	("button", ei_app_root_widget(), NULL, NULL);
	ei_anchor_t anchor = ei_anc_west;

    // ei_anchor_t img_anchor = ei_anc_center;
	// ei_surface_t image = hw_image_load(image_filename, ei_app_root_surface());

	ei_button_configure		(button, &((ei_size_t){300, 200}),
						&(ei_color_t){0x88, 0x88, 0x88, 0xff},
					 	&(int){6},
					 	&(int){30},
					 	&(ei_relief_t){ei_relief_raised},
					 	NULL, NULL,
					 	&(ei_color_t){0x00, 0x00, 0x00, 0xff}, NULL, NULL, NULL, NULL,
					 	&(ei_callback_t){button_press}, NULL);
	// ei_place_xy			(button, 150, 200);
	ei_place(button, &anchor, & (int){150}, & (int){300}, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_widget_t toplevel= ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
    ei_place_xy(toplevel, 100, 100);


	/* Run the application's main loop. */
    	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	
	// free(rect);


	return (EXIT_SUCCESS);
}
