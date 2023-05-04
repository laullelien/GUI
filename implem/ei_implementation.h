/**
 * @file	ei_implementation.h
 *
 * @brief 	Private definitions.
 *
 */

#ifndef EI_IMPLEMENTATION_H
#define EI_IMPLEMENTATION_H

#include "hw_interface.h"
#include "ei_types.h"
#include "ei_widget.h"

typedef struct segment
{
	int y_max;
	int x_y_min;
	uint16_t e;
	uint16_t dx;
	uint16_t dy;
	struct segment *next;
} segment;

/**
 * \brief	Allows to get the indices in order to minimize the size of TC
 *
 * @param	point_array 	The array of points defining the polygon. Its size is provided in the
 * 				next parameter (point_array_size). The array can be empty (i.e. nothing
 * 				is drawn) or else it must have more than 2 points.
 * @param	point_array_size The number of points in the point_array. Must be 0 or more than 2.
 *
 * @return 	Returns an array of size 2, with the minimum index first and then the maximum index.
 */
int *ei_TC_length(ei_point_t *point_array,
				  size_t point_array_size);

/**
 * \brief	Converts the red, green, blue and alpha components of a color into a 32 bits integer
 * 		than can be written directly in the memory returned by \ref hw_surface_get_buffer.
 * 		The surface parameter provides the channel order.
 *
 * @param	TC				Array of segment*, placed in function of y_min, the lowest y coordinate of the 2 points
 * @param	point_array 	The array of points defining the polygon. Its size is provided in the
 * 				next parameter (point_array_size). The array can be empty (i.e. nothing
 * 				is drawn) or else it must have more than 2 points.
 * @param	point_array_size The number of points in the point_array. Must be 0 or more than 2.
 * @param   TC_min The y idex of the lowest point
 *
 * @return 			The 32 bit integer corresponding to the color. The alpha component
 *				of the color is ignored in the case of surfaces that don't have an
 *				alpha channel.
 */

void ei_fill_TC(segment **TC, ei_point_t *point_array, size_t point_array_size, int TC_min);

uint32_t ei_impl_map_rgba(ei_surface_t surface, ei_color_t color);

/**
 * \brief	A structure storing the placement parameters of a widget.
 *		You have to define this structure: no suggestion provided.
 */
struct ei_impl_placer_params_t;

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
	ei_color_t *pick_color;			   ///< pick_id encoded as a color.
	void *user_data;				   ///< Pointer provided by the programmer for private use. May be NULL.
	ei_widget_destructor_t destructor; ///< Pointer to the programmer's function to call before destroying this widget. May be NULL.

	/* Widget Hierachy Management */
	ei_widget_t parent;		   ///< Pointer to the parent of this widget.
	ei_widget_t children_head; ///< Pointer to the first child of this widget.	Children are chained with the "next_sibling" field.
	ei_widget_t children_tail; ///< Pointer to the last child of this widget.
	ei_widget_t next_sibling;  ///< Pointer to the next child of this widget's parent widget.

	/* Geometry Management */
	//	ei_impl_placer_params_t* placer_params;	///< Pointer to the placer parameters for this widget. If NULL, the widget is not currently managed and thus, is not displayed on the screen.
	ei_size_t requested_size;  ///< Size requested by the widget (big enough for its label, for example), or by the programmer. This can be different than its screen size defined by the placer.
	ei_rect_t screen_location; ///< Position and size of the widget expressed in the root window reference.
	ei_rect_t *content_rect;   ///< Where to place children, when this widget is used as a container. By defaults, points to the screen_location.
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

#endif
