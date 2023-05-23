/**
 * @file	ei_implementation.h
 *
 * @brief 	Private definitions and implementation of widgets.
 *
 */

#ifndef EI_IMPLEMENTATION_H
#define EI_IMPLEMENTATION_H

#include "hw_interface.h"
#include "ei_types.h"
#include "ei_widget.h"



/**
 * \brief	Gets the id of a widget corresponding to the color given a surface.
 * 			The id is rearrange following the sequence of red, then green, then blue.
 *
 * @param 	surface		The surface which we collect the color from.
 * @param 	color		The color to be converted to an id.
 * 
 * @return The id corresponding to the color given a surface.
 */
uint32_t ei_get_id(ei_surface_t surface, uint32_t * color);


/**
 * \brief	Transforms the color into a chain of 32 bits following the correct order 
 * 			depending on the surface.
 *
 * @param 	surface		The surface which we want to color.
 * @param 	color		The color to be filled into the surface.
 * 
 * @return A chain of 32 bit corresponding to the correct order of color based on the surface.
 */
uint32_t ei_impl_map_rgba(ei_surface_t surface, ei_color_t color);


/**
 * \brief	A structure storing the placement parameters of a widget.
 *		You have to define this structure: no suggestion provided.
 */
typedef struct ei_impl_placer_params_t
{
	ei_anchor_t anchor;
	int x;
	int y;
	int width;
	int height;
	float rel_x;
	float rel_y;
	float rel_width;
	float rel_height;

} ei_impl_placer_params_t;

/**
 * \brief	Tells the placer to recompute the geometry of a widget.
 *		The widget must have been previsouly placed by a call to \ref ei_place.
 *		Geometry re-computation is necessary for example when the text label of
 *		a widget has changed, and thus the widget "natural" size has changed.
 *
 * @param	widget		The widget which geometry must be re-computed.
 */
void ei_impl_placer_run(ei_widget_t widget);

/**
 * \brief	Fields common to all types of widget. Every widget classes specializes this base
 *		class by adding its own fields.
 */
typedef struct ei_impl_widget_t
{
	ei_widgetclass_t *wclass;		   ///< The class of widget of this widget. Avoids the field name "class" which is a keyword in C++.
	uint32_t pick_id;				   ///< Id of this widget in the picking offscreen.
	//ei_color_t *pick_color;			   ///< pick_id encoded as a color.
	void *user_data;				   ///< Pointer provided by the programmer for private use. May be NULL.
	ei_widget_destructor_t destructor; ///< Pointer to the programmer's function to call before destroying this widget. May be NULL.

	/* Widget Hierachy Management */
	ei_widget_t parent;		   ///< Pointer to the parent of this widget.
	ei_widget_t children_head; ///< Pointer to the first child of this widget.	Children are chained with the "next_sibling" field.
	ei_widget_t children_tail; ///< Pointer to the last child of this widget.
	ei_widget_t next_sibling;  ///< Pointer to the next child of this widget's parent widget.

	/* Geometry Management */
	ei_impl_placer_params_t *placer_params; ///< Pointer to the placer parameters for this widget. If NULL, the widget is not currently managed and thus, is not displayed on the screen.
	ei_size_t requested_size;				///< Size requested by the widget (big enough for its label, for example), or by the programmer. This can be different than its screen size defined by the placer.
	ei_rect_t screen_location;				///< Position and size of the widget expressed in the root window reference.
	ei_rect_t content_rect;				///< Where to place children, when this widget is used as a container. By defaults, points to the screen_location.
} ei_impl_widget_t;

/**
 * @brief	Draws the children of a widget.
 * 		The children are draw withing the limits of the clipper and
 * 		the widget's content_rect.
 *
 * @param	widget		The widget which children are drawn.
 * @param	surface		A locked surface where to draw the widget's children.
 * @param	pick_surface	The picking offscreen.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference frame).
 */
void ei_impl_widget_draw_children(ei_widget_t widget,
								  ei_surface_t surface,
								  ei_surface_t pick_surface,
								  ei_rect_t *clipper);


/**
 * \brief	Returns a rectangle corresponding to the intersection of two clippers.
 *
 * @param 	first_clipper		The first clipper.
 * @param 	second_clipper		The second clipper.
 * 
 * @return A rectangle corresponding to the intersection of two clippers.
 */
ei_rect_t *ei_intersect_clipper(ei_rect_t *first_clipper, ei_rect_t *second_clipper);


/**
 * \brief	Returns the minimum of two integer.
 *
 * @param 	a	The first integer.
 * @param 	b	The second integer.
 * 
 * @return An integer corresponding to the minimum of two integers.
 */
int min(int a, int b);

#endif
