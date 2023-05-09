#include "../api/ei_draw.h"
#include "../api/hw_interface.h"
#include "../api/ei_types.h"
#include <stdint.h>
#include "ei_implementation.h"
#include <stdio.h>

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

    ei_borders borders[1];
    if (clipper != 0)
    {
        ei_initialize_borders(clipper, borders);
    }

    // if (clipper == NULL)
    // {
    //     for (int i = 0; i < width * height; i++)
    //     {
    //         *(p_first_pixel + i) = pixel_color;
    //     }
    // }

    int i = 0;
    ei_point_t point = {0, 0};

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            point.x = x;
            point.y = y;
            if (ei_inside_clipper(&point, clipper, borders))
            {
                *(p_first_pixel + i) = pixel_color;
            }
            i++;
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

    int dx = last_point->x - first_point->x;
    int dy = last_point->y - first_point->y;

    int e = 0;
    int offset = width * first_point->y + first_point->x;

    ei_borders borders[1];
    if (clipper != 0)
    {
        ei_initialize_borders(clipper, borders);
    }

    /* We first check if the line is horizontal*/
    if (dy == 0)
    {
        /* for the case where the two points are the same*/
        if (dx == 0 && ei_inside_clipper(first_point, clipper, borders))
        {
            *(p_first_pixel + offset) = pixel_color;
        }
        while (first_point->x < last_point->x)
        {
            if (ei_inside_clipper(first_point, clipper, borders))
            {
                *(p_first_pixel + offset) = pixel_color;
            }
            first_point->x++;
            offset++;
        }
        while (first_point->x > last_point->x)
        {
            if (ei_inside_clipper(first_point, clipper, borders))
            {
                *(p_first_pixel + offset) = pixel_color;
            }
            first_point->x--;
            offset--;
        }
        return;
    }

    /* We check if the line is vertical */
    if (dx == 0)
    {
        while (first_point->y > last_point->y)
        {
            if (ei_inside_clipper(first_point, clipper, borders))
            {
                *(p_first_pixel + offset) = pixel_color;
            }
            first_point->y--;
            offset -= width;
        }
        while (first_point->y < last_point->y)
        {
            if (ei_inside_clipper(first_point, clipper, borders))
            {
                *(p_first_pixel + offset) = pixel_color;
            }
            first_point->y++;
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
                while (first_point->y > last_point->y)
                {
                    if (ei_inside_clipper(first_point, clipper, borders))
                    {
                        *(p_first_pixel + offset) = pixel_color;
                    }
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
                return;
            }

            /* 2nd case */
            /* dx >= -dy, dx > 0, dy < 0 */
            if (dx >= -dy)
            {
                while (first_point->x < last_point->x)
                {
                    if (ei_inside_clipper(first_point, clipper, borders))
                    {
                        *(p_first_pixel + offset) = pixel_color;
                    }
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
                    if (ei_inside_clipper(first_point, clipper, borders))
                    {
                        *(p_first_pixel + offset) = pixel_color;
                    }
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
                return;
            }

            /* 4th case */
            /* dx < dy, dx > 0, dy > 0 */
            if (dx < dy)
            {
                while (first_point->y < last_point->y)
                {
                    if (ei_inside_clipper(first_point, clipper, borders))
                    {
                        *(p_first_pixel + offset) = pixel_color;
                    }
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
                    if (ei_inside_clipper(first_point, clipper, borders))
                    {
                        *(p_first_pixel + offset) = pixel_color;
                    }
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
                return;
            }

            /* 6th case */
            /* dx <= -dy, dx < 0, dy > 0 */
            if (dx <= -dy)
            {
                while (first_point->x > last_point->x)
                {
                    if (ei_inside_clipper(first_point, clipper, borders))
                    {
                        *(p_first_pixel + offset) = pixel_color;
                    }
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
                    if (ei_inside_clipper(first_point, clipper, borders))
                    {
                        *(p_first_pixel + offset) = pixel_color;
                    }
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
                return;
            }

            /* 8th case */
            /* dx > dy, dx < 0, dy < 0 */
            if (dx > dy)
            {
                while (first_point->y > last_point->y)
                {
                    if (ei_inside_clipper(first_point, clipper, borders))
                    {
                        *(p_first_pixel + offset) = pixel_color;
                    }
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
        ei_borders borders[1];
        if (clipper != 0)
        {
            borders->left = (clipper->top_left).x;
            borders->right = (clipper->top_left).x + (clipper->size).width;
            borders->upper = (clipper->top_left).y;
            borders->lower = (clipper->top_left).y + (clipper->size).height;
        }

        uint32_t pixel_color = ei_impl_map_rgba(surface, color);
        int width = hw_surface_get_size(surface).width;

        /* We determine how long TC must be */
        int *TC_length = ei_TC_length(point_array, point_array_size);
        /* We initialize and fill TC */
        ei_segment *TC[TC_length[1] - TC_length[0]];
        for (int i = 0; i <= TC_length[1] - TC_length[0]; i++)
        {
            TC[i] = 0;
        }
        ei_TC_fill(TC, point_array, point_array_size, TC_length[0]);
        // for (int i = 0; i <= TC_length[1] - TC_length[0]; i++)
        // {
        //     if (TC[i] != NULL)
        //     {
        //         segment *p_curr_seg = TC[i];
        //         while (p_curr_seg != NULL)
        //         {
        //             printf("line: %d, x_y_min : %d,y_max : %d\n", i, p_curr_seg->x_y_min, p_curr_seg->y_max);
        //             p_curr_seg = p_curr_seg->next;
        //         }
        //     }
        // }
        ei_segment *TCA = 0;
        ei_segment *p_curr_seg;
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
                ei_draw_scanline(surface, TCA, clipper, pixel_color, width, scanline + TC_length[0], borders);
                // segment *p_curr_seg = TCA;
                // while (p_curr_seg != NULL)
                // {
                //     if(p_curr_seg->dx>0)
                //     {
                //         printf("line: %d, x_y_min : %d,y_max : %d, e : %d, dx : %d, x: %f\n", scanline, p_curr_seg->x_y_min, p_curr_seg->y_max, p_curr_seg->e, p_curr_seg->dx, (float)p_curr_seg->x_y_min+(float)p_curr_seg->e/(float)p_curr_seg->dy);
                //     }
                //     else
                //     {
                //         printf("line: %d, x_y_min : %d,y_max : %d, e : %d, dx : %d, x: %f\n" , scanline, p_curr_seg->x_y_min, p_curr_seg->y_max, p_curr_seg->e, p_curr_seg->dx, (float)p_curr_seg->x_y_min-(float)p_curr_seg->e/(float)p_curr_seg->dy );

                //     }
                //     p_curr_seg = p_curr_seg->next;
                // }
                ei_update(TCA);
            }
        }
        free(TC_length);
        ei_TCA_free(TCA);
    }
}




/* angles in radian !!*/
void list_of_points_for_an_arc( ei_point_t center,
                                int radius,
                                float start_angle,
                                float end_angle,
                                int * length,
                                ei_point_t* list_points_arc)
{
    int start = (int)  (start_angle  * 180)/M_PI;
    if (start != 0){start++;}
    int end = (int)  (end_angle  * 180)/M_PI + 1;

    int nb_octant = (int) (end - start)/45;


    int nb_point = (int) radius * 0.70710678118 + 1;


    int x=0;
    int y = radius;
    int m = 5 - (radius<<2);
    int idx1 = 0;
    int idx3 = 0;
    int idx5 = 0;
    int idx7 = 0;
    int idx2 = nb_point - 1;
    int idx4 = nb_point - 1;
    int idx6 = nb_point - 1;
    int idx8 = nb_point - 1;
    int start_i;
    ei_point_t pts_a_tracer;
    while (x <= y)
    {

        for (int i = 0; i<nb_octant; i++)
        {
            start_i = start + i*45;
            /* 1er octant */
            if (90 <= start_i && start_i < 135)
            {
                pts_a_tracer.x = center.x - x;
                pts_a_tracer.y = center.y - y;
                //ei_draw_pixel(main_window,&pts_a_tracer,color,NULL);
                list_points_arc[idx1+i*nb_point] = pts_a_tracer;
                idx1++;
            }

            /* 2eme octant */
            if (135 <= start_i && start_i < 180)
            {
                pts_a_tracer.x = center.x - y;
                pts_a_tracer.y = center.y - x;
                //ei_draw_pixel(main_window,&pts_a_tracer,color,NULL);
                list_points_arc[idx2+i*nb_point] = pts_a_tracer;
                idx2--;
            }

            /* 3eme octant */
            if (180 <= start_i && start_i < 225)
            {
                pts_a_tracer.x = center.x - y;
                pts_a_tracer.y = center.y + x;
                //ei_draw_pixel(main_window,&pts_a_tracer,color,NULL);
                list_points_arc[idx3+i*nb_point] = pts_a_tracer;
                idx3++;
            }

            /* 4eme octant */
            if (225 <= start_i && start_i < 270)
            {
                pts_a_tracer.x = center.x - x;
                pts_a_tracer.y = center.y + y;
                //ei_draw_pixel(main_window,&pts_a_tracer,color,NULL);
                list_points_arc[idx4+i*nb_point] = pts_a_tracer;
                idx4--;

            }

            /* 5eme octant */
            if (270 <= start_i && start_i < 315)
            {
                pts_a_tracer.x = center.x + x;
                pts_a_tracer.y = center.y + y;
                //ei_draw_pixel(main_window,&pts_a_tracer,color,NULL);
                list_points_arc[idx5+i*nb_point] = pts_a_tracer;
                idx5++;

            }


            /* 6eme octant */
            if(315 <= start_i && start_i < 360)
            {
                pts_a_tracer.x = center.x + y;
                pts_a_tracer.y = center.y + x;
                //ei_draw_pixel(main_window,&pts_a_tracer,color,NULL);
                list_points_arc[idx6+i*nb_point] = pts_a_tracer;
                idx6--;
            }


            /* 7eme octant */
            if (0 <= start_i && start_i < 45)
            {
                pts_a_tracer.x = center.x + y;
                pts_a_tracer.y = center.y - x;
                //ei_draw_pixel(main_window,&pts_a_tracer,color,NULL);
                list_points_arc[idx7+i*nb_point] = pts_a_tracer;

                idx7++;
            }


            /* 8eme octant */
            if (45 <= start_i && start_i < 90)
            {
                pts_a_tracer.x = center.x + x;
                pts_a_tracer.y = center.y - y;
                //ei_draw_pixel(main_window,&pts_a_tracer,color,NULL);
                list_points_arc[idx8+i*nb_point] = pts_a_tracer;
                idx8--;
            }
        }


        if (m > 0)
        {
            y = y - 1;
            m = m- (y<<3);
        }
        x += 1;
        m = m + (x<<3) + 4;
    }

    *length = nb_octant*nb_point;

    return;
}


ei_point_t* list_of_points_for_a_rounded_frame(ei_rect_t rectangle,
                                               int radius,
                                               int* array_size)
{
    int nb_points = ((int) (radius * 0.70710678118 + 1)) << 1 ;
    ei_point_t* list_points_arc = calloc(nb_points*4 + 9, sizeof(ei_point_t));

    int length = 0;
    int total_length = 0;
    ei_point_t point;


    /* top left */
    point.x = rectangle.top_left.x + radius;
    point.y = rectangle.top_left.y;
    *(list_points_arc+total_length) = point;
    total_length += 1;


    ei_point_t center = {rectangle.top_left.x + radius, rectangle.top_left.y + radius};
    list_of_points_for_an_arc(center, radius, M_PI/2, M_PI, &length, list_points_arc+total_length);
    puts("laa");
    total_length += length;



    point.x = rectangle.top_left.x;
    point.y = rectangle.top_left.y + radius;
    *(list_points_arc+total_length) = point;
    total_length += 1;



    /* bottom left */
    point.x = rectangle.top_left.x;
    point.y = rectangle.top_left.y + rectangle.size.height - radius;
    *(list_points_arc+total_length) = point;
    total_length += 1;

    center.x = rectangle.top_left.x + radius;
    center.y = rectangle.top_left.y + rectangle.size.height - radius;
    list_of_points_for_an_arc(center, radius, M_PI, 3*M_PI/2, &length, list_points_arc+total_length);
    total_length += length;

    point.x = rectangle.top_left.x + radius;
    point.y = rectangle.top_left.y + rectangle.size.height;
    *(list_points_arc+total_length) = point;
    total_length += 1;





    /* bottom right */
    point.x = rectangle.top_left.x + rectangle.size.width - radius;
    point.y = rectangle.top_left.y + rectangle.size.height;
    *(list_points_arc+total_length) = point;
    total_length += 1;

    center.x = rectangle.top_left.x + rectangle.size.width - radius;
    center.y = rectangle.top_left.y + rectangle.size.height - radius;
    list_of_points_for_an_arc(center, radius,3*M_PI/2, 2*M_PI,&length, list_points_arc+total_length);
    total_length += length;

    point.x = rectangle.top_left.x + rectangle.size.width;
    point.y = rectangle.top_left.y + rectangle.size.height - radius;
    *(list_points_arc+total_length) = point;
    total_length += 1;







    /* top right */
    point.x = rectangle.top_left.x + rectangle.size.width;
    point.y = rectangle.top_left.y + radius;
    *(list_points_arc+total_length) = point;
    total_length += 1;

    center.x = rectangle.top_left.x + rectangle.size.width - radius;
    center.y = rectangle.top_left.y + radius;
    list_of_points_for_an_arc(center, radius, 0, M_PI/2, &length, list_points_arc+total_length);
    total_length += length;

    point.x = rectangle.top_left.x + rectangle.size.width - radius;
    point.y = rectangle.top_left.y;
    *(list_points_arc+total_length) = point;
    total_length += 1;




    /* add the first point as the final point to complete the circle */
    point.x = rectangle.top_left.x + radius;
    point.y = rectangle.top_left.y;
    *(list_points_arc+total_length) = point;
    total_length += 1;

    *array_size = total_length;


    return list_points_arc; /* remember to free list points at the end */
}