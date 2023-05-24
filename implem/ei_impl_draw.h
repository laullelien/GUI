/**
 * @file	ei_impl_draw.h
 *
 * @brief 	Implements functions that draw polygons.
 *
 */

#ifndef EI_IMPL_DRAW
#define EI_IMPL_DRAW

#include "hw_interface.h"
#include "ei_types.h"
#include "ei_widget.h"

/**
 * \brief	A structure that stores information about a segment.
 */
typedef struct ei_segment
{
    int y_max;
    int x_y_min;
    int e;
    int dx;
    uint16_t dy;
    struct ei_segment *next;
} ei_segment;

/**
 * \brief	A structure that stores information about the borders.
 */
typedef struct ei_borders
{
    int left;
    int right;
    int upper;
    int lower;
} ei_borders;

/**
 * \brief	Returns the indices of the lowest and highest points in order to minimize the size of TC.
 *
 * @param	point_array 	The array of points defining the polygon. Its size is provided in the
 * 				next parameter (point_array_size). The array can be empty (i.e. nothing
 * 				is drawn) or else it must have more than 2 points.
 * @param	point_array_size The number of points in the point_array. Must be 0 or more than 2.
 *
 * @return 	An array of size 2. The first element is the index TC_min of the lowest point and the second
 *          element is TC_max, the index of the highest point.
 */
int *ei_TC_length(ei_point_t *point_array,
                  size_t point_array_size);

/**
 * \brief	Creates TC, an array where each element TC[i] contains a linked list of segments.
 * 			The segments in TC[i] have a lowest point with a y coordinate equal to i.
 *
 * @param	TC				Array of segment* that we fill
 * @param	point_array 	The array of points defining the polygon. Its size is provided in the
 * 				next parameter (point_array_size). The array can be empty (i.e. nothing
 * 				is drawn) or else it must have more than 2 points.
 * @param	point_array_size The number of points in the point_array. Must be 0 or more than 2.
 * @param   TC_min The y index of the lowest point
 */
void ei_TC_fill(ei_segment **TC, ei_point_t *point_array, size_t point_array_size, int TC_min);

/**
 * \brief	Adds the new segments of the current scanline i.e the elements of TC[scanline] into TCA
 * and removes the segments of TCA which are such that y_max = scanline
 *
 * @param	TC				Array of segment*, stored based on y_min, the lowest y coordinate of the 2 points
 * @param	p_TCA 		A pointer to the segments which are currently being used to determine where to draw our line.
 * It corresponds to every line that intersect the scanline apart from horizontal lines which are useless for the algorithm
 * @param	scanline 	The index corresponding to the current scanline.
 * scanline + TC_min corresponds to the actual x index of the line we are dealing with in the surface.
 *
 */
void ei_TCA_remove_merge(ei_segment **TC, ei_segment **p_TCA, uint16_t scanline, int TC_min);

/**
 * \brief	Draws the current scanline by drawing between each segment couple
 *
 * @param	surface 	Where to draw the text. The surface must be *locked* by
 *				\ref hw_surface_lock.
 * @param	TCA 		The segments which are currently being used to determine where to draw our line.
 * It corresponds to every line that intersect the scanline apart from horizontal lines which are useless for the algorithm
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle.
 * @param	pixel_color		The color of the polygon, returned by \ref ei_impl_map_rgba
 * @param	color		The color of the polygon
 * @param	width		The width of the window that we are drawing on
 * @param	line_idx		The index of the line that we are drawing on
 *
 */
void ei_draw_scanline(ei_surface_t surface, ei_segment *TCA, const ei_rect_t *clipper, uint32_t pixel_color, ei_color_t color, int width, int line_idx);

/**
 * \brief	Frees the segments of TCA
 *
 * @param	TCA 		The segments which are currently being used to determine where to draw our line.
 * It corresponds to every line that intersect the scanline apart from horizontal lines which are useless for the algorithm
 *
 */
void ei_update(ei_segment *TCA);

/**
 * \brief	Updates the x_y_min of the pixels in the current scanline
 *
 * @param	TCA 		The segments which are currently being used to determine where to draw our line.
 * It corresponds to every line that intersect the scanline apart from horizontal lines which are useless for the algorithm
 *
 */
void ei_TCA_free(ei_segment *TCA);

/**
 * @brief Sort the linked list TCA according to abscisses (by merging)
 *
 * @param first Pointer to the first cell of TCA
 *
 */
ei_segment *ei_TCA_sort(ei_segment *first);

/**
 * @brief   Initialize the borders to the clipper's borders
 *
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle.
 * @param	borders     The indices of the clipper extrmums coordinates i.e. x_min (left), x_max (right), y_min (upper), y_max (lower)
 */
void ei_initialize_borders(const ei_rect_t *clipper,
                           ei_borders *borders);

/**
 * @brief   Indicates whether the point to draw is inside the clipper.
 *
 * @param	point       The point to draw.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle.
 * @param	borders     The indices of the clipper extrmums coordinates i.e. x_min (left), x_max (right), y_min (upper), y_max (lower)
 *
 * @return 			A boolean that is true if and only if the point is inside the clipper
 */
bool ei_inside_clipper(ei_point_t *point,
                       const ei_rect_t *clipper,
                       ei_borders *borders);

/**
 * @brief   Analytical clipping of a line.
 *
 * @param   first_point The first point of the line.
 * @param   last_point  The second point of the line.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle.
 * @param	dx          Gradient with respect to x.
 * @param   dy          Gradient with respect to y.
 * @param   e           Secret to our success.
 *
 * @return 			A boolean that is true if and only if the point is inside the clipper
 */
bool ei_intersect_line_clipper(ei_point_t *first_point, ei_point_t *last_point, const ei_rect_t *clipper, int dx, int dy, int *e);

/**
 * \brief	Frees TC
 *
 * @param	TC				Array of segment*, stored based on y_min, the lowest y coordinate of the 2 points
 *
 */
void ei_TC_free(ei_segment **TC, int length, int first_unused_TC_line);

/**
 * \brief	Returns a rectangle corresponding to the intersection of two clippers.
 *
 * @param 	first_clipper		The first clipper.
 * @param 	second_clipper		The second clipper.
 *
 * @return A rectangle corresponding to the intersection of two clippers.
 */
ei_rect_t *ei_intersect_clipper(ei_rect_t *first_clipper, ei_rect_t *second_clipper);

#endif
