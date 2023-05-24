#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget_configure.h"
#include "../implem/ei_implementation.h"
#include "../implem/ei_impl_widget.h"



bool process_key(ei_event_t* event)
{
    if ( (event->type == ei_ev_close) ||
         ((event->type == ei_ev_keydown) && (event->param.key.key_code == SDLK_ESCAPE))) {
        ei_app_quit_request();
        return true;
    } else
        return false;
}



/*
 * ei_main --
 *
 *	Main function of the application.
 */

int main(int argc, char** argv)
{
    /* Create the application and change the color of the background. */
    ei_app_create			((ei_size_t){1000, 900}, false);
    ei_frame_set_bg_color		(ei_app_root_widget(), (ei_color_t){0x52, 0x7f, 0xb4, 0xff});
    ei_event_set_default_handle_func(process_key);


    bool top1 = true;
    bool top2 = true;
    bool top3 = true;
    bool top = true;

    ei_widget_t toplevel_1;
    ei_widget_t toplevel_2;
    ei_widget_t toplevel_3;

    void button_press(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param)
    {
        if (top) {


            if (((ei_widget_t) user_param) == toplevel_1 && top1) {
                { ei_widget_destroy((ei_widget_t) user_param); }
                top1 = false;
                top = false;
            }
            if (((ei_widget_t) user_param) == toplevel_2 && top2) {
                { ei_widget_destroy((ei_widget_t) user_param); }
                top2 = false;
            }
            if (((ei_widget_t) user_param) == toplevel_3 && top3) {
                { ei_widget_destroy((ei_widget_t) user_param); }
                top3 = false;
            }
        }

    }



    toplevel_1 = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
    bool closable = true;
    ei_toplevel_configure(toplevel_1, NULL, NULL, NULL, NULL, &closable, NULL, NULL);
    ei_place(toplevel_1, NULL, & (int){100}, & (int){100}, & (int){400}, & (int){400}, NULL, NULL, NULL, NULL);

    toplevel_2 = ei_widget_create("toplevel", toplevel_1, NULL, NULL);
    ei_toplevel_configure(toplevel_2, NULL, NULL, NULL, NULL, &closable, NULL, NULL);
    ei_place(toplevel_2, NULL, & (int){50}, & (int){50}, & (int){350}, & (int){350}, NULL, NULL, NULL, NULL);


    toplevel_3 = ei_widget_create("toplevel", toplevel_2, NULL, NULL);
    ei_toplevel_configure(toplevel_3, NULL, NULL, NULL, NULL, &closable, NULL, NULL);
    ei_place(toplevel_3, NULL, & (int){50}, & (int){50}, & (int){250}, & (int){250}, NULL, NULL, NULL, NULL);




    ei_widget_t button1 = ei_widget_create	("button", ei_app_root_widget(), NULL, NULL);
    ei_button_configure		(button1, NULL,
                                &(ei_color_t){0x88, 0x88, 0x88, 0xff},
                                &(int){6},
                                &(int){40},
                                &(ei_relief_t){ei_relief_raised},
                                &(ei_string_t){"First toplevel"}, NULL,
                                &(ei_color_t){0x00, 0x00, 0x00, 0xff}, NULL, NULL, NULL, NULL,
                                &(ei_callback_t){button_press}, (void *) &toplevel_1);
    ei_place(button1, NULL, & (int){120}, & (int){700}, & (int){150}, & (int){50}, NULL, NULL, NULL, NULL);



    ei_widget_t button2 = ei_widget_create	("button", ei_app_root_widget(), NULL, NULL);
    ei_button_configure		(button2, NULL,
                                &(ei_color_t){0x88, 0x88, 0x88, 0xff},
                                &(int){6},
                                &(int){40},
                                &(ei_relief_t){ei_relief_raised},
                                &(ei_string_t){"Second toplevel"}, NULL,
                                &(ei_color_t){0x00, 0x00, 0x00, 0xff}, NULL, NULL, NULL, NULL,
                                &(ei_callback_t){button_press}, (void *) &toplevel_2);
    ei_place(button2, NULL, & (int){420}, & (int){700}, & (int){150}, & (int){50}, NULL, NULL, NULL, NULL);



    ei_widget_t button3 = ei_widget_create	("button", ei_app_root_widget(), NULL, NULL);
    ei_button_configure		(button3, NULL,
                                &(ei_color_t){0x88, 0x88, 0x88, 0xff},
                                &(int){6},
                                &(int){40},
                                &(ei_relief_t){ei_relief_raised},
                                &(ei_string_t){"Third toplevel"}, NULL,
                                &(ei_color_t){0x00, 0x00, 0x00, 0xff}, NULL, NULL, NULL, NULL,
                                &(ei_callback_t){button_press}, (void *) &toplevel_3);
    ei_place(button3, NULL, & (int){720}, & (int){700}, & (int){150}, & (int){50}, NULL, NULL, NULL, NULL);



    /* Run the application's main loop. */
    ei_app_run();

    /* We just exited from the main loop. Terminate the application (cleanup). */
    ei_app_free();

    return (EXIT_SUCCESS);
}