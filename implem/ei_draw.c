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

int *ei_TC_length(ei_point_t *point_array, size_t point_array_size)
{
    int TC_length[2] = {point_array->y, point_array->y};
    for (size_t i = 1; i < point_array_size; i++)
    {
        if (point_array[i].y < TC_length[0])
        {
            TC_length[0] = point_array[i].y;
        }
        else if (point_array[i].y > TC_length[1])
        {
            TC_length[1] = point_array[i].y;
        }
    }
    return TC_length;
}

void ei_fill_TC(segment **TC, ei_point_t *point_array, size_t point_array_size, int TC_min)
{
    /* We store y_min to know in what line of TC we will ad it */
    int y_min;
    segment *p_curr_seg;
    for (size_t i = 0; i < point_array_size - 1; i++)
    {
        p_curr_seg = malloc(sizeof(segment));
        /* We check which point is higher */
        if (point_array[i].y < point_array[i + 1].y)
        {
            y_min = point_array[i].y;
            p_curr_seg->y_max = point_array[i + 1].y;
            p_curr_seg->x_y_min = point_array[i].x;
            p_curr_seg->dx = point_array[i + 1].x - point_array[i].x;
            p_curr_seg->dy = point_array[i + 1].y - point_array[i].y;
            p_curr_seg->e = 0;
        }
        else
        {
            y_min = point_array[i + 1].y;
            p_curr_seg->y_max = point_array[i].y;
            p_curr_seg->x_y_min = point_array[i + 1].x;
            p_curr_seg->dx = point_array[i].x - point_array[i + 1].x;
            p_curr_seg->dy = point_array[i].y - point_array[i + 1].y;
            p_curr_seg->e = 0;
        }
        segment *curr_line;
        /* We ignore horizontal lines */
        if (p_curr_seg->dx != 0)
        {
            /* We fill TC */
            curr_line = TC[y_min - TC_min];
            /* If this line is empty */
            if (curr_line == NULL)
            {
                TC[y_min - TC_min] = p_curr_seg;
            }
            else
            {
                /* We find where to add the current segment */
                while (curr_line->next != NULL)
                {
                    curr_line++;
                }
                curr_line->next = p_curr_seg;
            }
        }
    }
}

void ei_draw_polygon(ei_surface_t surface,
                     ei_point_t *point_array,
                     size_t point_array_size,
                     ei_color_t color,
                     const ei_rect_t *clipper)
{
    if (point_array_size != 0)
    {
        uint32_t *p_first_pixel = (uint32_t *)hw_surface_get_buffer(surface);
        uint32_t pixel_color = ei_impl_map_rgba(surface, color);
        int width = hw_surface_get_size(surface).width;
        int height = hw_surface_get_size(surface).height;

        /* We determine how long TC must be */
        int *TC_length = ei_TC_length(point_array, point_array_size);

        /* We initialize TC */
        segment *TC[TC_length[1] - TC_length[0] + 1];
        ei_fill_TC(segment * TC, ei_point_t * point_array, size_t point_array_size, int TC_min);

        segment *TCA;
        /* We update TCA */
        for (uint16_t scanline = 0; scanline <= TC_max - TC_min; scanline++)
        {
            if (TCA == NULL)
            {
                TCA = TC[scanline];
            }
            /* if TCA is still NULL, we stop */
            if (TCA != NULL)
            {
                p_prev_seg = *TCA;
                p_curr_seg = p_prev_seg->next;
                /* There is only one segment*/
                if (p_prev_seg == NULL)
                {
                    if (p_curr_seg->y_max == scanline)
                    {
                        TCA = TC[scanline];
                    }
                    else
                    {
                        p_prev_seg->next = TC[scanline];
                    }
                }
                /* We know there are at least 2 segments */
                else
                {
                    while (p_curr_seg->next != 0)
                    {
                        if (p_curr_seg->y_max == scanline)
                        {
                            /* We remove the segment */
                            p_prev_seg->next = p_curr_seg->next;
                            free(p_cur_seg);
                        }
                        p_prev_seg = p_prev_seg->next;
                        p_curr_seg = p_prev_seg->next;
                    }
                    /* Now, there is one last segment to check */
                    if (p_curr_seg->y_max == scanline)
                    {
                        p_prev_seg->next = TC[scanline];
                    }
                    else
                    {
                        p_curr_seg->next = TC[scanline];
                    }
                }
            }
        }
        TCA = linked_list_merge_sort(TCA);
    }
}
