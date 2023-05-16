

#ifndef PROJETC_IG_EI_DRAW_ARC_H
#define PROJETC_IG_EI_DRAW_ARC_H

#include "hw_interface.h"
#include "ei_types.h"
#include "ei_widget.h"



/**
 * \brief	fill in list_points_arc the points that represent the arc
 *
 * @param	center the center of the arc to draw
 * @param   radius the radius of the arc
 * @param   start_angle the angle to begin the arc form
 * @param   end_angle the angle to finish the arc form
 * @param   length the number of points to draw the arc
 * @param   list_points_arc the fill-in-the-blank list of arc points
 *
 * @return 	void
 */
void list_of_points_for_an_arc( ei_point_t center,
                                int radius,
                                float start_angle,
                                float end_angle,
                                int * length,
                                ei_point_t* list_points_arc);



/**
 * \brief	Returns the indices of the lowest and highest points in order to minimize the size of TC.
 *
 * @param	rectangle the position of the rectangle
 * @param   radius the radius of the corners
 * @param   height the height h to draw the relief
 * @param   array_size the size of the list of points
 *
 * @return 	a pointer to a list of points
 */
ei_point_t* list_of_points_for_a_rounded_frame(ei_rect_t rectangle,
                                               int radius,
                                               int* array_size,
                                               int area,
                                               bool is_horizontal);


/**
 * \brief	draw the button by using draw_polygone and list_of_points_for_a_rounded_frame twice
 *
 * @param	surface the surface to draw
 * @param   rectangle the rectangle taht represent the button
 *
 * @return 	void
 */
void ei_draw_button(ei_surface_t surface,
                    ei_rect_t rectangle, 
                    ei_color_t main_color, 
                    ei_relief_t relief,  
                    int border_width, 
                    int radius,
                    ei_rect_t *clipper,
                    ei_surface_t picksurface,
                    ei_color_t pick_color);


#endif //PROJETC_IG_EI_DRAW_ARC_H
