#include "../api/ei_draw.h"
#include "../api/hw_interface.h"
#include "../api/ei_types.h"
#include <stdint.h>
#include "ei_implementation.h"
#include <stdio.h>

bool is_inside(ei_point_t point,
               int left_border,
               int right_border,
               int upper_border,
               int lower_border)
{
    return (point.x >= left_border) && (point.x <= right_border) && (point.y >= upper_border) && (point.y <= lower_border);
}

bool is_inside_clipper(ei_point_t point,
                       const ei_rect_t *clipper)
{
    if (clipper == NULL)
    {
        return true;
    }
    int left_border = (clipper->top_left).x;
    int right_border = left_border + (clipper->size).width;
    int upper_border = (clipper->top_left).y;
    int lower_border = upper_border + (clipper->size).height;

    return is_inside(point, left_border, right_border, upper_border, lower_border);
}

void ei_draw_pixel(ei_surface_t surface,
                   ei_point_t *point,
                   ei_color_t color,
                   const ei_rect_t *clipper)
{

    int width = hw_surface_get_size(surface).width;
    uint32_t *p_first_pixel = (uint32_t *)hw_surface_get_buffer(surface);
    uint32_t pixel_color = ei_impl_map_rgba(surface, color);
    if (is_inside_clipper(*point, clipper))
    {
        *(p_first_pixel + (width * point->y + point->x)) = pixel_color;
    }
}

void ei_fill(ei_surface_t surface,
             const ei_color_t *color,
             const ei_rect_t *clipper)
{
    int width = hw_surface_get_size(surface).width;
    int height = hw_surface_get_size(surface).height;

    uint32_t *p_first_pixel = (uint32_t *)hw_surface_get_buffer(surface);
    uint32_t pixel_color = ei_impl_map_rgba(surface, *color);

    if (clipper == NULL)
    {
        for (int i = 0; i < width * height; i++)
        {
            *(p_first_pixel + i) = pixel_color;
        }
    }
    else
    {
        int left_border = (clipper->top_left).x;
        int right_border = left_border + (clipper->size).width;
        int upper_border = (clipper->top_left).y;
        int lower_border = upper_border + (clipper->size).height;
        int i = 0;
        ei_point_t point = {0, 0};

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                point.x = x;
                point.y = y;
                if (is_inside(point, left_border, right_border, upper_border, lower_border))
                {
                    *(p_first_pixel + i) = pixel_color;
                }
                i++;
            }
        }
    }
}

void ei_draw_line(ei_surface_t surface,
                  ei_point_t *first_point,
                  ei_point_t *last_point,
                  ei_color_t color,
                  const ei_rect_t *clipper)
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

    if (clipper == NULL)
    {
        /* We first check if the line is horizontal*/
        if (dy == 0)
        {
            /* for the case where the two points are the same*/
            if (x0 == x1)
            {
                ei_draw_pixel(surface, first_point, color, clipper);
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
    else
    {
        ei_point_t point_to_verify;
        int left_border = (clipper->top_left).x;
        int right_border = left_border + (clipper->size).width;
        int upper_border = (clipper->top_left).y;
        int lower_border = upper_border + (clipper->size).height;

        /* We first check if the line is horizontal*/
        if (dy == 0)
        {
            /* for the case where the two points are the same*/
            if (x0 == x1)
            {
                ei_draw_pixel(surface, first_point, color, clipper);
            }
            while (x0 < x1)
            {
                point_to_verify.x = x0;
                point_to_verify.y = y0;
                if (is_inside(point_to_verify, left_border, right_border, upper_border, lower_border))
                {
                    *(p_first_pixel + offset) = pixel_color;
                }
                x0++;
                offset++;
            }
            while (x0 > x1)
            {
                point_to_verify.x = x0;
                point_to_verify.y = y0;
                if (is_inside(point_to_verify, left_border, right_border, upper_border, lower_border))
                {
                    *(p_first_pixel + offset) = pixel_color;
                }
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
                point_to_verify.x = x0;
                point_to_verify.y = y0;
                if (is_inside(point_to_verify, left_border, right_border, upper_border, lower_border))
                {
                    *(p_first_pixel + offset) = pixel_color;
                }
                y0--;
                offset -= width;
            }
            while (y0 < y1)
            {
                point_to_verify.x = x0;
                point_to_verify.y = y0;
                if (is_inside(point_to_verify, left_border, right_border, upper_border, lower_border))
                {
                    *(p_first_pixel + offset) = pixel_color;
                }
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
                        point_to_verify.x = x0;
                        point_to_verify.y = y0;
                        if (is_inside(point_to_verify, left_border, right_border, upper_border, lower_border))
                        {
                            *(p_first_pixel + offset) = pixel_color;
                        }
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
                        point_to_verify.x = x0;
                        point_to_verify.y = y0;
                        if (is_inside(point_to_verify, left_border, right_border, upper_border, lower_border))
                        {
                            *(p_first_pixel + offset) = pixel_color;
                        }
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
                        point_to_verify.x = x0;
                        point_to_verify.y = y0;
                        if (is_inside(point_to_verify, left_border, right_border, upper_border, lower_border))
                        {
                            *(p_first_pixel + offset) = pixel_color;
                        }
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
                        point_to_verify.x = x0;
                        point_to_verify.y = y0;
                        if (is_inside(point_to_verify, left_border, right_border, upper_border, lower_border))
                        {
                            *(p_first_pixel + offset) = pixel_color;
                        }
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
                        point_to_verify.x = x0;
                        point_to_verify.y = y0;
                        if (is_inside(point_to_verify, left_border, right_border, upper_border, lower_border))
                        {
                            *(p_first_pixel + offset) = pixel_color;
                        }
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
                        point_to_verify.x = x0;
                        point_to_verify.y = y0;
                        if (is_inside(point_to_verify, left_border, right_border, upper_border, lower_border))
                        {
                            *(p_first_pixel + offset) = pixel_color;
                        }
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
                        point_to_verify.x = x0;
                        point_to_verify.y = y0;
                        if (is_inside(point_to_verify, left_border, right_border, upper_border, lower_border))
                        {
                            *(p_first_pixel + offset) = pixel_color;
                        }
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
                        point_to_verify.x = x0;
                        point_to_verify.y = y0;
                        if (is_inside(point_to_verify, left_border, right_border, upper_border, lower_border))
                        {
                            *(p_first_pixel + offset) = pixel_color;
                        }
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

    if (point_array_size == 1)
    {
        ei_draw_pixel(surface, point_array, color, clipper);
        return;
    }
    for (uint32_t i = 0; i < point_array_size - 1; i++)
    {
        ei_draw_line(surface, point_array, point_array + 1, color, clipper);
        point_array++;
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
        /* We determine how long TC must be */
        int *TC_length = ei_TC_length(point_array, point_array_size);
        /* We initialize and fill TC */
        segment *TC[TC_length[1] - TC_length[0]];
        for (int i = 0; i <= TC_length[1] - TC_length[0]; i++)
        {
            TC[i] = 0;
        }
        ei_TC_fill(TC, point_array, point_array_size, TC_length[0]);
        segment *TCA = 0;
        segment *p_curr_seg;
        /* We update TCA and draw for each scanline */
        for (uint16_t scanline = 0; scanline < TC_length[1] - TC_length[0]; scanline++)
        {
            ei_TCA_remove_merge(TC, &TCA, scanline, TC_length[0]);
            p_curr_seg = TCA;
            while (p_curr_seg != 0)
            {
                p_curr_seg = p_curr_seg->next;
            }
            if (TCA != NULL)
            {
                TCA = ei_TCA_sort(TCA);
                ei_draw_scanline(TCA, surface, color, TC_length, scanline + TC_length[0]);
                ei_update(TCA);
            }
        }
        free(TC_length);
        ei_TCA_free(TCA);
    }
};