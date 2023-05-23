/**
 * @file	ei_impl_event.h
 *
 * @brief 	Implements functions to recognise widgets and to merge invalidate rectangles.
 *
 */


#ifndef EI_IMPL_EVENT_H
#define EI_IMPL_EVENT_H

#include "ei_types.h"


/**
 * \brief	return the widget pointed by the mouse on the screen
 *
 * @param	event the current event
 * @param   pick_surface the picksurface in order to recognize the right widget
 *
 * @return 	a widget
 */
ei_widget_t get_widget_from_mouse_location(ei_event_t *event, ei_surface_t pick_surface);

/**
 * \brief	transform the linked list rects in a linked list with one big rectangle
 *          that includes others rectangle if the sum of the areas of each rectangle
 *          is greater than the great rectangle
 *
 * @param	rects a linked list of invalid rects
 *
 * @return 	void
 */
void merge_rect_clipper(ei_linked_rect_t * rects);



#endif
