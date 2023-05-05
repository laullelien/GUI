#include "../api/ei_draw.h"
#include "../api/hw_interface.h"
#include "../api/ei_types.h"
#include <stdint.h>
#include "ei_implementation.h"
#include <stdio.h>

/* Pour le moment, on ne prend pas en compte le clipping*/

void ei_draw_pixel(ei_surface_t surface,
                   ei_point_t *point,
                   ei_color_t color)
{

    int width = hw_surface_get_size(surface).width;
    uint32_t *p_first_pixel = (uint32_t *)hw_surface_get_buffer(surface);
    uint32_t pixel_color = ei_impl_map_rgba(surface, color);
    *(p_first_pixel + (width * point->y + point->x)) = pixel_color;
}

void ei_fill(ei_surface_t surface,
             const ei_color_t *color,
             const ei_rect_t *clipper)
{

    if (clipper == NULL)
    {
        int width = hw_surface_get_size(surface).width;
        int heigth = hw_surface_get_size(surface).height;

        uint32_t *p_first_pixel = (uint32_t *)hw_surface_get_buffer(surface);
        uint32_t pixel_color = ei_impl_map_rgba(surface, *color);

        for (int i = 0; i < width * heigth; i++)
        {
            *(p_first_pixel + i) = pixel_color;
        }
    }
}

void ei_draw_line(ei_surface_t surface,
                  ei_point_t *first_point,
                  ei_point_t *last_point,
                  ei_color_t color)
{

    uint32_t *p_first_pixel = (uint32_t *)hw_surface_get_buffer(surface);
    uint32_t pixel_color = ei_impl_map_rgba(surface, color);
    int width = hw_surface_get_size(surface).width;

    int x0 = first_point->x;
    int x1 = last_point->x;
    int dx = x1 - x0;

    int y0 = first_point->y;
    int y1 = last_point->y;
    int dy = y1 - y0;

    int e = 0;
    int offset = width * y0 + x0;

    /* We first check if the line is horizontal*/
    if (dy == 0)
    {
        /* for the case where the two points are the same*/
        if (x0 == x1)
        {
            ei_draw_pixel(surface, first_point, color);
        }
        while (x0 < x1)
        {
            *(p_first_pixel + offset) = pixel_color;
            x0++;
            offset++;
        }
        while (x0 > x1)
        {
            *(p_first_pixel + offset) = pixel_color;
            x0--;
            offset--;
        }
        return;
    }

    /* We check if the line is vertical */
    if (dx == 0)
    {
        int y0 = first_point->y;
        int y1 = last_point->y;
        while (y0 > y1)
        {
            *(p_first_pixel + offset) = pixel_color;
            y0--;
            offset -= width;
        }
        while (y0 < y1)
        {
            *(p_first_pixel + offset) = pixel_color;
            y0++;
            offset += width;
        }
        return;
    }

    /* We start from noon and go clockwise */
    /* right half*/
    if (dx > 0)
    {
        /* top right part */
        if (dy < 0)
        {

            /* 1st case */
            /* dx < -dy, dx > 0, dy < 0 */
            if (dx < -dy)
            {
                while (y0 > y1)
                {
                    *(p_first_pixel + offset) = pixel_color;
                    y0--;
                    e += dx;
                    offset -= width;
                    if (e << 1 > dy)
                    {
                        x0++;
                        e += dy;
                        offset++;
                    }
                }
                return;
            }

            /* 2nd case */
            /* dx >= -dy, dx > 0, dy < 0 */
            if (dx >= -dy)
            {
                while (x0 < x1)
                {
                    *(p_first_pixel + offset) = pixel_color;
                    x0++;
                    e -= dy;
                    offset++;
                    if (e << 1 > dx)
                    {
                        y0--;
                        e -= dx;
                        offset -= width;
                    }
                }
                return;
            }
        }

        /* bottom right*/
        else
        {
            /* 3rd case */
            /* dx >= dy, dx > 0, dy > 0 */
            if (dx >= dy)
            {
                while (x0 < x1)
                {
                    *(p_first_pixel + offset) = pixel_color;
                    x0++;
                    e += dy;
                    offset++;
                    if (e << 1 > dx)
                    {
                        y0++;
                        e -= dx;
                        offset += width;
                    }
                }
                return;
            }

            /* 4th case */
            /* dx < dy, dx > 0, dy > 0 */
            if (dx < dy)
            {
                while (y0 < y1)
                {
                    *(p_first_pixel + offset) = pixel_color;
                    y0++;
                    e += dx;
                    offset += width;
                    if (e << 1 > dy)
                    {
                        x0++;
                        e -= dy;
                        offset++;
                    }
                }
                return;
            }
        }
    }
    /* Now dx < 0 */
    /* left half */
    else
    {
        /* bottom left part */
        if (dy > 0)
        {

            /* 5th case */
            /* dx > -dy, dx < 0, dy > 0 */
            if (dx > -dy)
            {
                while (y0 < y1)
                {
                    *(p_first_pixel + offset) = pixel_color;
                    y0++;
                    e -= dx;
                    offset += width;
                    if (e << 1 > dx)
                    {
                        x0--;
                        e -= dy;
                        offset--;
                    }
                }
                return;
            }

            /* 6th case */
            /* dx <= -dy, dx < 0, dy > 0 */
            if (dx <= -dy)
            {
                while (x0 > x1)
                {
                    *(p_first_pixel + offset) = pixel_color;
                    x0--;
                    e += dy;
                    offset--;
                    if (e << 1 > -dx)
                    {
                        y0++;
                        e += dx;
                        offset += width;
                    }
                }
                return;
            }
        }

        /* top left*/
        else
        {
            /* 7th case */
            /* dx <= dy, dx < 0, dy < 0 */
            if (dx <= dy)
            {
                while (x0 > x1)
                {
                    *(p_first_pixel + offset) = pixel_color;
                    x0--;
                    e -= dy;
                    offset--;
                    if (e << 1 > dx)
                    {
                        y0--;
                        e += dx;
                        offset -= width;
                    }
                }
                return;
            }

            /* 8th case */
            /* dx > dy, dx < 0, dy < 0 */
            if (dx > dy)
            {
                while (y0 > y1)
                {
                    *(p_first_pixel + offset) = pixel_color;
                    y0--;
                    e -= dx;
                    offset -= width;
                    if (e << 1 > dy)
                    {
                        x0--;
                        e += dy;
                        offset--;
                    }
                }
                return;
            }
        }
    }
}

void ei_draw_polyline(ei_surface_t surface,
                      ei_point_t *point_array,
                      size_t point_array_size,
                      ei_color_t color,
                      const ei_rect_t *clipper)
{
    if (point_array_size == 0)
    {
        return;
    }

    if (clipper == NULL)
    {

        if (point_array_size == 1)
        {
            ei_draw_pixel(surface, point_array, color);
        }
        for (uint32_t i = 0; i < point_array_size - 1; i++)
        {           
            ei_draw_line(surface, point_array, point_array + 1, color);
            point_array++;
        }
    }
}


// void	ei_draw_polygon		(ei_surface_t		surface,
// 				 ei_point_t*		point_array,
// 				 size_t			point_array_size,
// 				 ei_color_t		color,
// 				 const ei_rect_t*	clipper)
// {
//     if(point_array_size!=0)
//     {
//         uint32_t *p_first_pixel = (uint32_t *)hw_surface_get_buffer(surface);
//         uint32_t pixel_color = ei_impl_map_rgba(surface, color);
//         int width = hw_surface_get_size(surface).width;
//         int height = hw_surface_get_size(surface).height;
//         /* We determine how long TC must be */
//         int TC_min=point_array->y;
//         int TC_max=point_array->y;
//         for(size_t i=1; i<point_array_size; i++)
//         {
//             if (point_array[i].y< TC_min)
//             {
//                 TC_min=point_array[i].y;
//             }
//             else if(point_array[i].y > TC_max)
//             {
//                 TC_max=point_array[i].y;
//             }
//         }
//         segment* TC[TC_max-TC_min+1];
//         segment* curr_seg;
//         segment* prev_seg;
//         for(size_t i=0; i<point_array_size-1; i++)
//         {
//             curr_seg=malloc(sizeof(segment));
//             /* We check which point is higher */
//             if(point_array[i].y<point_array[i+1].y)
//             {
//                 curr_seg->y_max=point_array[i+1].y;
//                 curr_seg->x_y_min=point_array[i+1].x;
//             }
//             else
//             {
//                 curr_seg->y_max=point_array[i].y;
//                 curr_seg->x_y_min=point_array[i].x;
//             }
//             curr_seg->dx=point_array[i].x-point_array[i+1].x;
//             curr_seg->dy=point_array[i].y-point_array[i+1].y;
//             curr_seg->e=0;
//         }
//     }
// }



