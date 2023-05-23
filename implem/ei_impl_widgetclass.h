/**
 *  @file	ei_impl_widgetclass.h
 * 
 *  @brief	Implement specific structures and functions for each type of widget: frame, button and toplevel. 
 * 
 */

#ifndef EI_IMPL_WIDGETCLASS_H
#define EI_IMPL_WIDGETCLASS_H

#include "../api/ei_widgetclass.h"
#include "ei_implementation.h"
#include "ei_types.h"


/* FRAME */


/**
 * \brief	A structure that stores information about the frame class.
 */
typedef struct ei_impl_frame_t
{
    ei_impl_widget_t ei_frame_widget;
    ei_color_t color;
    int border_width;
    ei_relief_t relief;
    ei_string_t text;
    ei_font_t text_font;
    ei_color_t text_color;
    ei_anchor_t text_anchor;
    ei_surface_t img;
    ei_rect_ptr_t img_rect;
    ei_anchor_t img_anchor;
} ei_impl_frame_t;


/**
 * \brief	A function that allocates a block of memory that is big enough to store the
 *		attributes of a frame widget: both the common attributes, that use
 *		\ref ei_widget_struct_size bytes, and the specific attributes of the class.
 *		After allocation, this function initialize the memory to 0.
 *
 * @return			A block of memory to represent a frame widget, with all bytes set to 0.
 */
ei_widget_t ei_frame_allocfunc();


/**
 * \brief	A function that releases the memory used by a frame widget before it is destroyed.
 *		The memory used for the \ref ei_widget_t structure itself is *not* freed by the
 *		function. It frees the *fields* of the structure.
 *
 * @param	frame		The frame which resources are to be freed.
 */
void ei_frame_releasefunc(ei_widget_t frame);


/**
 * \brief	A function that draws a frame widget.
 * 		The function also draw the children of the widget.
 *
 * @param	frame		A pointer to the frame instance to draw.
 * @param	surface		A locked surface where to draw the widget. The actual location of the widget in the
 *				surface is stored in its "screen_location" field.
 * @param	pick_surface	The picking offscreen.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference frame).
 */
void ei_frame_drawfunc(ei_widget_t frame,
                       ei_surface_t surface,
                       ei_surface_t pick_surface,
                       ei_rect_t *clipper);


/**
 * \brief	A function that sets the default values for a frame widget.
 *
 * @param	widget		A pointer to the frame widget instance to initialize.
 */
void ei_frame_setdefaultsfunc(ei_widget_t frame);


/**
 * \brief	A function that draws text of a frame.
 *
 * @param	surface		A locked surface where to draw the text of the frame.
 * @param   frame       A pointer to the frame instance.
 * @param   clipper     If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference frame).
 */
void ei_draw_frame_text(ei_surface_t surface, ei_widget_t frame, ei_rect_t *clipper);


/**
 * \brief	A function that draws image of a frame.
 *
 * @param	surface		A locked surface where to draw the image of the frame.
 * @param   frame       A pointer to the frame instance.
 * @param   clipper     If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference frame).
 */
void ei_draw_frame_img(ei_surface_t surface, ei_widget_t frame, ei_rect_t *clipper);


/**
 * \brief	A function that initialises the frame widgetclass from widgetclass list with the 
 * right functions in \ref ei_widgetclass_register.
 *
 * @param	ei_frame_widgetclass		A pointer to the frame widgetclass to initialize.
 */
void ei_frame_widgetclass_create(ei_widgetclass_t *ei_frame_widgetclass);




/* BUTTON */

/**
 * \brief	A structure that stores information about the frame class.
 */
typedef struct ei_impl_button_t
{
    ei_impl_widget_t button_widget;
    ei_color_t color;
    int border_width;
    int corner_radius;
    ei_relief_t relief;
    ei_string_t text;
    ei_font_t text_font;
    ei_color_t text_color;
    ei_anchor_t text_anchor;
    ei_surface_t img;
    ei_rect_ptr_t img_rect;
    ei_anchor_t img_anchor;
    ei_callback_t callback;
    ei_user_param_t user_param;
} ei_impl_button_t;


/**
 * \brief	A function that allocates a block of memory that is big enough to store the
 *		attributes of a button widget: both the common attributes, that use
 *		\ref ei_widget_struct_size bytes, and the specific attributes of the class.
 *		After allocation, this function initialize the memory to 0.
 *
 * @return			A block of memory to represent a button widget, with all bytes set to 0.
 */
ei_widget_t ei_button_allocfunc();


/**
 * \brief	A function that draws a button widget.
 * 		The function also draw the children of the widget.
 *
 * @param	button		A pointer to the button instance to draw.
 * @param	surface		A locked surface where to draw the widget. The actual location of the widget in the
 *				surface is stored in its "screen_location" field.
 * @param	pick_surface	The picking offscreen.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference button).
 */
void ei_button_drawfunc(ei_widget_t button,
                        ei_surface_t surface,
                        ei_surface_t pick_surface,
                        ei_rect_t *clipper);


/**
 * \brief	A function that sets the default values for a button widget.
 *
 * @param	widget		A pointer to the button widget instance to initialize.
 */
void ei_button_setdefaultsfunc(ei_widget_t button);


/**
 * \brief	A function that draws text of a button.
 *
 * @param	surface		A locked surface where to draw the text of the button.
 * @param   button       A pointer to the button instance.
 * @param   clipper     If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference button).
 */
void ei_draw_button_text(ei_surface_t surface, ei_widget_t button, ei_rect_t *clipper);


/**
 * \brief	A function that draws image of a button.
 *
 * @param	surface		A locked surface where to draw the image of the button.
 * @param   button       A pointer to the button instance.
 * @param   clipper     If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference button).
 */
void ei_draw_button_img(ei_surface_t surface, ei_widget_t button, ei_rect_t *clipper);


/**
 * \brief	A function that initialises the button widgetclass from widgetclass list with the 
 * right functions in \ref ei_widgetclass_register.
 *
 * @param	ei_frame_widgetclass		A pointer to the button widgetclass to initialize.
 */
void ei_button_widgetclass_create(ei_widgetclass_t *ei_frame_widgetclass);



/* TOPLEVEL */


/**
 * \brief	A structure that stores information about the frame class.
 */
typedef struct ei_impl_toplevel_t
{
    ei_impl_widget_t toplevel_widget;
    ei_color_t color;
    int border_width;
    ei_string_t title;
    bool closable;
    ei_axis_set_t resizable;
    ei_size_ptr_t min_size;
} ei_impl_toplevel_t;


/**
 * \brief	A function that allocates a block of memory that is big enough to store the
 *		attributes of a toplevel widget: both the common attributes, that use
 *		\ref ei_widget_struct_size bytes, and the specific attributes of the class.
 *		After allocation, this function initialize the memory to 0.
 *
 * @return			A block of memory to represent a toplevel widget, with all bytes set to 0.
 */
ei_widget_t ei_toplevel_allocfunc();


/**
 * \brief	A function that releases the memory used by a toplevel widget before it is destroyed.
 *		The memory used for the \ref ei_widget_t structure itself is *not* freed by the
 *		function. It frees the *fields* of the structure.
 *
 * @param	toplevel		The toplevel which resources are to be freed.
 */
void ei_toplevel_releasefunc(ei_widget_t toplevel);


/**
 * \brief	A function that draws a toplevel widget.
 * 		The function also draw the children of the widget.
 *
 * @param	toplevel	A pointer to the toplevel instance to draw.
 * @param	surface		A locked surface where to draw the widget. The actual location of the widget in the
 *				surface is stored in its "screen_location" field.
 * @param	pick_surface	The picking offscreen.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference toplevel).
 */
void ei_toplevel_drawfunc(ei_widget_t toplevel,
                          ei_surface_t surface,
                          ei_surface_t pick_surface,
                          ei_rect_t *clipper);


/**
 * \brief	A function that sets the default values for a toplevel widget.
 *
 * @param	widget		A pointer to the toplevel widget instance to initialize.
 */
void ei_toplevel_setdefaultsfunc(ei_widget_t toplevel);


/**
 * \brief	A function that initialises the toplevel widgetclass from widgetclass list with the 
 * right functions in \ref ei_widgetclass_register.
 *
 * @param	ei_frame_widgetclass		A pointer to the toplevel widgetclass to initialize.
 */
void ei_toplevel_widgetclass_create(ei_widgetclass_t *ei_toplevel_widgetclass);


#endif