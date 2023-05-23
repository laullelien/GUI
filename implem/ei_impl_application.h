/**
 *  @file	ei_impl_application.h
 * 
 *  @brief	Implements the functions needed for the list of invalidate rectangles to be updated on the
 *          main surface.
 * 
 */

#ifndef EI_IMPL_APPLICATION_H
#define EI_IMPL_APPLICATION_H

#include "ei_types.h"


/**
 * \brief	A function that returns the list of invalidate rectangles (pointer to the first cell of the list).
 *
 * @return  List of invalidate rectangles.
 */
ei_linked_rect_t* get_p_rect_cell(void);


/**
 * \brief	A function that returns the pointer to the list of invalidate rectangles (pointer of pointer
 *          to the first cell of the list).
 *
 * @return  A pointer to the list of invalidate rectangles.
 */
ei_linked_rect_t** get_pp_rect_cell(void);


/**
 * \brief	A function that frees the list of invalidate rectangles.
 */
void free_p_rect_cell(void);


#endif
