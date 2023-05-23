#include "../api/ei_draw.h"
#include "../api/hw_interface.h"
#include "../api/ei_types.h"
#include <stdint.h>
#include "ei_implementation.h"
#include <stdio.h>
#include "ei_impl_draw.h"

void ei_draw_pixel(ei_surface_t surface,
                   ei_point_t *point,
                   ei_color_t color,
                   const ei_rect_t *clipper)
{
    ei_borders borders[1];
    if (clipper != 0)
    {
        ei_initialize_borders(clipper, borders);
    }

    int width = hw_surface_get_size(surface).width;

    uint32_t *p_first_pixel = (uint32_t *)hw_surface_get_buffer(surface);
    uint32_t pixel_color = ei_impl_map_rgba(surface, color);

    if (ei_inside_clipper(point, clipper, borders))
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

    if (clipper != NULL)
    {
        ei_rect_t surface_rect = hw_surface_get_rect(surface);
        ei_rect_t clipper_rect = *clipper;
        ei_rect_t *p_intercept_rect = ei_intersect_clipper(&clipper_rect, &surface_rect);

        uint32_t *p_first_pixel_in_window = p_first_pixel + p_intercept_rect->top_left.x + p_intercept_rect->top_left.y * width;
        for (int y = 0; y < p_intercept_rect->size.height; y++)
        {
            for (int x = 0; x < p_intercept_rect->size.width; x++)
            {
                *p_first_pixel_in_window = pixel_color;
                p_first_pixel_in_window++;
            }
            p_first_pixel_in_window = p_first_pixel_in_window + width - p_intercept_rect->size.width;
        }
    }
    else
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                *p_first_pixel = pixel_color;
                p_first_pixel++;
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

    /* stores the coordinates to set points to their initial values at the end */
    ei_point_t first_temp = *first_point;
    ei_point_t last_temp = *last_point;

    int width = hw_surface_get_size(surface).width;
    int dx = last_point->x - first_point->x;
    int dy = last_point->y - first_point->y;
    int e = 0;
    // printf("f %i, %i, %i, %i\n", first_point->x, first_point->y, last_point->x, last_point->y);
    bool in;

    if (clipper)
    {
        in = ei_intersect_line_clipper(surface, first_point, last_point, clipper, dx, dy, &e);
    }
    else
    {
        ei_rect_t surface_rect = hw_surface_get_rect(surface);
        in = ei_intersect_line_clipper(surface, first_point, last_point, &surface_rect, dx, dy, &e);
    }
    // printf("then %i, %i, %i, %i\n", first_point->x, first_point->y, last_point->x, last_point->y);
    if (!in)
    {
        /* put back points to their initial values */
        *last_point = last_temp;
        *first_point = first_temp;
        return;
    }
    int offset = width * first_point->y + first_point->x;

    /* We first check if the line is horizontal*/
    if (dy == 0)
    {
        /* for the case where the two points are the same*/
        if (dx == 0)
        {
            *(p_first_pixel + offset) = pixel_color;
        }
        while (first_point->x < last_point->x)
        {
            *(p_first_pixel + offset) = pixel_color;
            first_point->x++;
            offset++;
        }
        while (first_point->x > last_point->x)
        {
            *(p_first_pixel + offset) = pixel_color;
            first_point->x--;
            offset--;
        }
        /* put back points to their initial values */
        *last_point = last_temp;
        *first_point = first_temp;
        return;
    }

    /* We check if the line is vertical */
    if (dx == 0)
    {
        while (first_point->y > last_point->y)
        {

            *(p_first_pixel + offset) = pixel_color;
            first_point->y--;
            offset -= width;
        }
        while (first_point->y < last_point->y)
        {

            *(p_first_pixel + offset) = pixel_color;
            first_point->y++;
            offset += width;
        }
        /* put back points to their initial values */
        *last_point = last_temp;
        *first_point = first_temp;
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
                while (first_point->y > last_point->y)
                {
                    *(p_first_pixel + offset) = pixel_color;
                    first_point->y--;
                    e += dx;
                    offset -= width;
                    if (e << 1 > -dy)
                    {
                        first_point->x++;
                        e += dy;
                        offset++;
                    }
                }
                /* put back points to their initial values */
                *last_point = last_temp;
                *first_point = first_temp;
                return;
            }

            /* 2nd case */
            /* dx >= -dy, dx > 0, dy < 0 */
            if (dx >= -dy)
            {
                while (first_point->x < last_point->x)
                {
                    *(p_first_pixel + offset) = pixel_color;
                    first_point->x++;
                    e -= dy;
                    offset++;
                    if (e << 1 > dx)
                    {
                        first_point->y--;
                        e -= dx;
                        offset -= width;
                    }
                }
                /* put back points to their initial values */
                *last_point = last_temp;
                *first_point = first_temp;
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
                while (first_point->x < last_point->x)
                {
                    *(p_first_pixel + offset) = pixel_color;
                    first_point->x++;
                    e += dy;
                    offset++;
                    if (e << 1 > dx)
                    {
                        first_point->y++;
                        e -= dx;
                        offset += width;
                    }
                }
                /* put back points to their initial values */
                *last_point = last_temp;
                *first_point = first_temp;
                return;
            }

            /* 4th case */
            /* dx < dy, dx > 0, dy > 0 */
            if (dx < dy)
            {
                while (first_point->y < last_point->y)
                {
                    *(p_first_pixel + offset) = pixel_color;
                    first_point->y++;
                    e += dx;
                    offset += width;
                    if (e << 1 > dy)
                    {
                        first_point->x++;
                        e -= dy;
                        offset++;
                    }
                }
                /* put back points to their initial values */
                *last_point = last_temp;
                *first_point = first_temp;
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
                while (first_point->y < last_point->y)
                {
                    *(p_first_pixel + offset) = pixel_color;
                    first_point->y++;
                    e -= dx;
                    offset += width;
                    if (e << 1 > dy)
                    {
                        first_point->x--;
                        e -= dy;
                        offset--;
                    }
                }
                /* put back points to their initial values */
                *last_point = last_temp;
                *first_point = first_temp;
                return;
            }

            /* 6th case */
            /* dx <= -dy, dx < 0, dy > 0 */
            if (dx <= -dy)
            {
                while (first_point->x > last_point->x)
                {
                    *(p_first_pixel + offset) = pixel_color;
                    first_point->x--;
                    e += dy;
                    offset--;
                    if (e << 1 > -dx)
                    {
                        first_point->y++;
                        e += dx;
                        offset += width;
                    }
                }
                /* put back points to their initial values */
                *last_point = last_temp;
                *first_point = first_temp;
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
                while (first_point->x > last_point->x)
                {
                    *(p_first_pixel + offset) = pixel_color;
                    first_point->x--;
                    e -= dy;
                    offset--;
                    if (e << 1 > -dx)
                    {
                        first_point->y--;
                        e += dx;
                        offset -= width;
                    }
                }
                /* put back points to their initial values */
                *last_point = last_temp;
                *first_point = first_temp;
                return;
            }

            /* 8th case */
            /* dx > dy, dx < 0, dy < 0 */
            if (dx > dy)
            {
                while (first_point->y > last_point->y)
                {
                    *(p_first_pixel + offset) = pixel_color;
                    first_point->y--;
                    e -= dx;
                    offset -= width;
                    if (e << 1 > -dy)
                    {
                        first_point->x--;
                        e += dy;
                        offset--;
                    }
                }
                /* put back points to their initial values */
                *last_point = last_temp;
                *first_point = first_temp;
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

        uint32_t pixel_color = ei_impl_map_rgba(surface, color);
        int width = hw_surface_get_size(surface).width;

        /* We determine how long TC must be */
        int *TC_length = ei_TC_length(point_array, point_array_size);

        /* We determine the minimum and maximum index scanlines */
        int max_line_idx = TC_length[1] < (clipper->top_left.y + clipper->size.height) ? (TC_length[1] - TC_length[0]) : (clipper->top_left.y + clipper->size.height - TC_length[0]);
        int min_line_idx = TC_length[0] > clipper->top_left.y ? 0 : (clipper->top_left.y - TC_length[0]);
        if (min_line_idx > max_line_idx)
        {
            free(TC_length);
            return;
        }

        /* We initialize and fill TC */
        ei_segment *TC[TC_length[1] - TC_length[0]];
        for (int i = 0; i <= TC_length[1] - TC_length[0]; i++)
        {
            TC[i] = 0;
        }
        ei_TC_fill(TC, point_array, point_array_size, TC_length[0]);
        ei_segment *TCA = 0;

        /* We just update TCA until we need to draw */
        for (uint16_t scanline = 0; scanline < min_line_idx; scanline++)
        {
            ei_TCA_remove_merge(TC, &TCA, scanline, TC_length[0]);
            if (TCA != NULL)
            {
                ei_update(TCA);
            }
        }

        /* We update TCA and draw for each scanline */
        for (uint16_t scanline = min_line_idx; scanline < max_line_idx; scanline++)
        {
            ei_TCA_remove_merge(TC, &TCA, scanline, TC_length[0]);
            if (TCA != NULL)
            {
                TCA = ei_TCA_sort(TCA);
                ei_draw_scanline(surface, TCA, clipper, pixel_color, width, scanline + TC_length[0]);
                ei_update(TCA);
            }
        }
        ei_TCA_free(TCA);
        ei_TC_free(TC, TC_length[1] - TC_length[0], max_line_idx);
        free(TC_length);
    }
}

int ei_copy_surface(ei_surface_t destination,
                    const ei_rect_t *dst_rect,
                    ei_surface_t source,
                    const ei_rect_t *src_rect,
                    bool alpha)
{
    int dst_height, dst_width, src_height, src_width;
    int dst_offset, src_offset;

    if (dst_rect == NULL)
    {
        dst_height = hw_surface_get_size(destination).height;
        dst_width = hw_surface_get_size(destination).width;
        dst_offset = 0;
    }
    else
    {
        dst_height = dst_rect->size.height;
        dst_width = dst_rect->size.width;
        dst_offset = hw_surface_get_size(destination).width * dst_rect->top_left.y + dst_rect->top_left.x;
    }
    if (src_rect == NULL)
    {
        src_height = hw_surface_get_size(source).height;
        src_width = hw_surface_get_size(source).width;
        src_offset = 0;
    }
    else
    {
        src_height = src_rect->size.height;
        src_width = src_rect->size.width;
        src_offset = hw_surface_get_size(source).width * src_rect->top_left.y + src_rect->top_left.x;
    }

    // if the dst_rect and src_rest have different size
    if ((dst_height != src_height) || (dst_width != src_width))
    {
        return 1;
    }
    uint8_t *src_start_addr = hw_surface_get_buffer(source) + (src_offset << 2);
    uint8_t *dst_start_addr = hw_surface_get_buffer(destination) + (dst_offset << 2);

    // int i = 0;
    // int j = 0;
    int ir, ig, ib, ia;
    hw_surface_get_channel_indices(source, &ir, &ig, &ib, &ia);
    // the final pixels are an exact copy of the source pixels, including the alpha channel
    if (alpha == false || ia == -1)
    {
        for (int y = 0; y < src_height; y++)
        {
            for (int x = 0; x < src_width; x++)
            {
                *((uint32_t *)dst_start_addr) = *((uint32_t *)src_start_addr);
                dst_start_addr += 4;
                src_start_addr += 4;
            }
            dst_start_addr += (hw_surface_get_size(destination).width - src_width) << 2;
            src_start_addr += (hw_surface_get_size(source).width - src_width) << 2;
        }
    }
    else
    {
        uint16_t src_alpha;
        for (int y = 0; y < src_height; y++)
        {
            for (int x = 0; x < src_width; x++)
            {
                src_alpha = (uint16_t)(*(src_start_addr + ia));
                src_alpha++;
                *(dst_start_addr + ia) = 255;
                *(dst_start_addr + ir) = (uint8_t)(((uint16_t)(*(dst_start_addr + ir)) * (255 - src_alpha) + (uint16_t)(*(src_start_addr + ir)) * src_alpha) / 255);
                *(dst_start_addr + ig) = (uint8_t)(((uint16_t)(*(dst_start_addr + ig)) * (255 - src_alpha) + (uint16_t)(*(src_start_addr + ig)) * src_alpha) / 255);
                *(dst_start_addr + ib) = (uint8_t)(((uint16_t)(*(dst_start_addr + ib)) * (255 - src_alpha) + (uint16_t)(*(src_start_addr + ib)) * src_alpha) / 255);
                dst_start_addr += 4;
                src_start_addr += 4;
            }
            dst_start_addr += (hw_surface_get_size(destination).width - src_width) << 2;
            src_start_addr += (hw_surface_get_size(source).width - src_width) << 2;
        }
    }
    return 0;
}

void ei_draw_text(ei_surface_t surface,
                  const ei_point_t *where,
                  ei_const_string_t text,
                  ei_font_t font,
                  ei_color_t color,
                  const ei_rect_t *clipper)
{
    ei_surface_t text_surface = hw_text_create_surface(text, font, color);
    hw_surface_lock(text_surface);
    ei_rect_t dst_rect;
    ei_rect_t src_rect;
    src_rect.top_left = (ei_point_t){0, 0};
    dst_rect.top_left = *where;
    hw_text_compute_size(text, font, &dst_rect.size.width, &dst_rect.size.height);

    if (clipper != NULL)
    {
        /* intersect clipper and dst_rect and apply same modifications on src_rect */
        if (dst_rect.size.width + dst_rect.top_left.x > clipper->size.width + clipper->top_left.x)
        {
            dst_rect.size.width -= dst_rect.size.width + dst_rect.top_left.x - clipper->size.width - clipper->top_left.x;
        }
        if (dst_rect.size.height + dst_rect.top_left.y > clipper->size.height + clipper->top_left.y)
        {
            dst_rect.size.height -= dst_rect.size.height + dst_rect.top_left.y - clipper->size.height - clipper->top_left.y;
        }
        if (dst_rect.top_left.x < clipper->top_left.x)
        {
            dst_rect.size.width -= clipper->top_left.x - dst_rect.top_left.x;
            src_rect.top_left.x += clipper->top_left.x - dst_rect.top_left.x;
            dst_rect.top_left.x = clipper->top_left.x;
        }
        if (dst_rect.top_left.y < clipper->top_left.y)
        {
            dst_rect.size.height -= clipper->top_left.y - dst_rect.top_left.y;
            src_rect.top_left.y += clipper->top_left.y - dst_rect.top_left.y;
            dst_rect.top_left.y = clipper->top_left.y;
        }
    }
    src_rect.size = dst_rect.size;
    if (dst_rect.size.width > 0 && dst_rect.size.height > 0)
    {
        ei_copy_surface(surface, &dst_rect, text_surface, &src_rect, true);
    }
    hw_surface_unlock(text_surface);
    hw_surface_free(text_surface);
}