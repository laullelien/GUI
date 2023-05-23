

#ifndef PROJETC_IG_EI_DRAW_ARC_H
#define PROJETC_IG_EI_DRAW_ARC_H

#include "hw_interface.h"
#include "ei_types.h"
#include "ei_widget.h"



/**
 * \brief	fills in list_points_arc the points that represent the arc
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
 * \brief	returns a pointer to a list of points that represent a rounded frame
 *
 * @param	rectangle the position of the rectangle
 * @param   radius the radius of the corners
 * @param   array_size the size of the list of points
 * @param   area the type of area into the rounded frame to draw
 * @param   is_horizontal represent a vertical or horizontal rounded frame
 *
 * @return 	a pointer to a list of points
 */
ei_point_t* list_of_points_for_a_rounded_frame(ei_rect_t rectangle,
                                               int radius,
                                               int* array_size,
                                               int area,
                                               bool is_horizontal);


/**
 * \brief	draws the button by using draw_polygone and list_of_points_for_a_rounded_frame twice
 *
 * @param	surface the surface to draw
 * @param   rectangle the rectangle that represent the button
 * @param   main_color main color of the button
 * @param   relief type of relief of the button, none, sunken or raised
 * @param   border_width border width of the button
 * @param   radius the radius at the corner
 * @param   clipper clipper to clipping the button
 * @param   picksurface the surface that represent the picksurface
 * @param   pick_color the color of the button in the picksurface
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
