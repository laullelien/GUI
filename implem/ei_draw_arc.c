#include "ei_draw_arc.h"
#include <stdio.h>
#include "ei_implementation.h"


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
                                               int height,
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