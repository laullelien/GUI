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
    fprintf(stdout, "nboctant = %i\n", nb_octant);

    int nb_point = ((int) (radius * 0.70710678118 + 1)) ;


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
                fprintf(stdout, " avant %i\n", idx1+i*nb_point);

                idx1++;
            }

            /* 2eme octant */
            if (135 <= start_i && start_i < 180)
            {
                pts_a_tracer.x = center.x - y;
                pts_a_tracer.y = center.y - x;
                //ei_draw_pixel(main_window,&pts_a_tracer,color,NULL);
                list_points_arc[idx2+i*nb_point] = pts_a_tracer;
                fprintf(stdout, "%i\n", idx2+i*nb_point);

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

    *length = nb_point*nb_octant;

    return;
}










ei_point_t* list_of_points_for_a_rounded_frame(ei_rect_t rectangle,
                                               int radius,
                                               int* array_size,
                                               int area)
{
    int height = (int) ( (rectangle.size.height) >> 1);
    int nb_points = ((int) (radius * 0.70710678118 + 1)) ;  //for one octant

    int width_half = (int) nb_points;

    int length = 0;
    int total_length = 0;
    ei_point_t point;
    ei_point_t center;

    /* TOP PART */
    if (area == 0)
    {
        ei_point_t* list_points_arc = calloc(nb_points*4 + 9, sizeof(ei_point_t));
        /* top right */

        point.x = rectangle.top_left.x + rectangle.size.width - radius + width_half;
        point.y = rectangle.top_left.y + radius - width_half;
        *(list_points_arc+total_length) = point;
        total_length += 1;



        center.x = rectangle.top_left.x + rectangle.size.width - radius;
        center.y = rectangle.top_left.y + radius;
        list_of_points_for_an_arc(center, radius, M_PI/4, M_PI/2, &length, list_points_arc+total_length);
        total_length += length;


        point.x = rectangle.top_left.x + rectangle.size.width - radius;
        point.y = rectangle.top_left.y;
        *(list_points_arc+total_length) = point;
        total_length += 1;





        /* top left */
        point.x = rectangle.top_left.x + radius;
        point.y = rectangle.top_left.y;
        *(list_points_arc+total_length) = point;
        total_length += 1;




        center.x = rectangle.top_left.x + radius;
        center.y = rectangle.top_left.y + radius;

        list_of_points_for_an_arc(center, radius, M_PI/2, M_PI, &length, list_points_arc+total_length);
        total_length += length;


        point.x = rectangle.top_left.x;
        point.y = rectangle.top_left.y + radius;
        *(list_points_arc+total_length) = point;
        total_length += 1;




        /* We begin the form right here */




        /* bottom left */
        point.x = rectangle.top_left.x;
        point.y = rectangle.top_left.y + rectangle.size.height - radius;
        *(list_points_arc+total_length) = point;
        total_length += 1;


        center.x = rectangle.top_left.x + radius;
        center.y = rectangle.top_left.y + rectangle.size.height - radius;
        list_of_points_for_an_arc(center, radius, M_PI, 5*M_PI/4, &length, list_points_arc+total_length);
        total_length += length;


        point.x = rectangle.top_left.x + radius - width_half;
        point.y = rectangle.top_left.y + rectangle.size.height - radius + width_half;
        *(list_points_arc+total_length) = point;
        total_length += 1;


        ei_point_t height_1 = {rectangle.top_left.x + height, rectangle.top_left.y + height};
        *(list_points_arc+total_length) = height_1;
        total_length += 1;

        ei_point_t height_2 = {rectangle.top_left.x + rectangle.size.width - height, rectangle.top_left.y + height};
        *(list_points_arc+total_length) = height_2;
        total_length += 1;


        point.x = rectangle.top_left.x + rectangle.size.width - radius + width_half;
        point.y = rectangle.top_left.y + radius - width_half;
        *(list_points_arc+total_length) = point;
        total_length += 1;


        *array_size = total_length;
        return list_points_arc; /* remember to free list points at the end */
    }
    /* BOTTOM PART */
    else if (area == 1)
    {
        ei_point_t* list_points_arc = calloc(nb_points*4 + 9, sizeof(ei_point_t));
        /* top right */
        point.x = rectangle.top_left.x + rectangle.size.width - radius + width_half;
        point.y = rectangle.top_left.y + radius - width_half;
        *(list_points_arc+total_length) = point;
        total_length += 1;



        ei_point_t height_1;
        height_1.x = rectangle.top_left.x + rectangle.size.width - height;
        height_1.y = rectangle.top_left.y + height;
        *(list_points_arc+total_length) = height_1;
        total_length += 1;


        ei_point_t height_2 = {rectangle.top_left.x + height, rectangle.top_left.y + height};
        *(list_points_arc+total_length) = height_2;
        total_length += 1;

        point.x = rectangle.top_left.x + radius - width_half;
        point.y = rectangle.top_left.y + rectangle.size.height - radius + width_half;
        *(list_points_arc+total_length) = point;
        total_length += 1;




        center.x = rectangle.top_left.x + radius;
        center.y = rectangle.top_left.y + rectangle.size.height - radius;

        list_of_points_for_an_arc(center, radius,5*M_PI/4, 3*M_PI/2,&length, list_points_arc+total_length);
        total_length += length;

        point.x = rectangle.top_left.x + radius;
        point.y = rectangle.top_left.y + rectangle.size.height ;
        *(list_points_arc+total_length) = point;
        total_length += 1;


        point.x = rectangle.top_left.x + rectangle.size.width - radius;
        point.y = rectangle.top_left.y + rectangle.size.height ;
        *(list_points_arc+total_length) = point;
        total_length += 1;




        center.x = rectangle.top_left.x +  rectangle.size.width - radius;
        center.y = rectangle.top_left.y + rectangle.size.height - radius;
        list_of_points_for_an_arc(center, radius,3*M_PI/2, 2*M_PI,&length, list_points_arc+total_length);
        total_length += length;



        point.x = rectangle.top_left.x + rectangle.size.width;
        point.y = rectangle.top_left.y + rectangle.size.height - radius;
        *(list_points_arc+total_length) = point;
        total_length += 1;



        point.x = rectangle.top_left.x + rectangle.size.width;
        point.y = rectangle.top_left.y + radius;
        *(list_points_arc+total_length) = point;
        total_length += 1;


        center.x = rectangle.top_left.x +  rectangle.size.width - radius;
        center.y = rectangle.top_left.y + radius;
        list_of_points_for_an_arc(center, radius,0, M_PI/4,&length, list_points_arc+total_length);
        total_length += length;


        point.x = rectangle.top_left.x + rectangle.size.width - radius + width_half;
        point.y = rectangle.top_left.y + radius - width_half;
        *(list_points_arc+total_length) = point;
        total_length += 1;


        *array_size = total_length;
        return list_points_arc; /* remember to free list points at the end */
    }
    /* MIDDLE PART */
    else
    {
        ei_point_t* list_points_arc = calloc(nb_points*8 + 9, sizeof(ei_point_t));

        /* TOP LEFT */
        point.x = rectangle.top_left.x + radius ;
        point.y = rectangle.top_left.y ;
        *(list_points_arc+total_length) = point;
        total_length += 1;


        center.x = rectangle.top_left.x +  radius;
        center.y = rectangle.top_left.y + radius;
        list_of_points_for_an_arc(center, radius,M_PI/2, M_PI,&length, list_points_arc+total_length);
        total_length += length;


        point.x = rectangle.top_left.x ;
        point.y = rectangle.top_left.y + radius;
        *(list_points_arc+total_length) = point;
        total_length += 1;



        /* BOTTOM LEFT */
        point.x = rectangle.top_left.x ;
        point.y = rectangle.top_left.y + rectangle.size.height - radius;
        *(list_points_arc+total_length) = point;
        total_length += 1;

        center.x = rectangle.top_left.x +  radius;
        center.y = rectangle.top_left.y + rectangle.size.height - radius;
        list_of_points_for_an_arc(center, radius,M_PI, 3*M_PI/2,&length, list_points_arc+total_length);
        total_length += length;

        point.x = rectangle.top_left.x + radius;
        point.y = rectangle.top_left.y + rectangle.size.height ;
        *(list_points_arc+total_length) = point;
        total_length += 1;


        /* BOTTOM RIGHT */
        point.x = rectangle.top_left.x +rectangle.size.width - radius;
        point.y = rectangle.top_left.y + rectangle.size.height ;
        *(list_points_arc+total_length) = point;
        total_length += 1;

        center.x = rectangle.top_left.x +  rectangle.size.width - radius;
        center.y = rectangle.top_left.y + rectangle.size.height - radius;
        list_of_points_for_an_arc(center, radius, 3*M_PI/2, 2*M_PI,&length, list_points_arc+total_length);
        total_length += length;

        point.x = rectangle.top_left.x +rectangle.size.width ;
        point.y = rectangle.top_left.y + rectangle.size.height - radius ;
        *(list_points_arc+total_length) = point;
        total_length += 1;


        /* TOP RIGHT */
        point.x = rectangle.top_left.x +rectangle.size.width ;
        point.y = rectangle.top_left.y + radius ;
        *(list_points_arc+total_length) = point;
        total_length += 1;

        center.x = rectangle.top_left.x +  rectangle.size.width - radius;
        center.y = rectangle.top_left.y + radius;
        list_of_points_for_an_arc(center, radius, 0, M_PI/2,&length, list_points_arc+total_length);
        total_length += length;

        point.x = rectangle.top_left.x + rectangle.size.width - radius;
        point.y = rectangle.top_left.y ;
        *(list_points_arc+total_length) = point;
        total_length += 1;

        point.x = rectangle.top_left.x + radius;
        point.y = rectangle.top_left.y;
        *(list_points_arc+total_length) = point;
        total_length += 1;

        *array_size = total_length;
        return list_points_arc; /* remember to free list points at the end */
    }


}


void ei_draw_button(ei_surface_t surface,ei_rect_t rectangle)
{
    int radius1 = 30;
    int radius2 = 20;

    int length1;
    int length2;
    int length3;

    ei_rect_t middle_rectangle;
    middle_rectangle.top_left.x = rectangle.top_left.x + 15;
    middle_rectangle.top_left.y = rectangle.top_left.y + 15;
    middle_rectangle.size.height = rectangle.size.height - 30;
    middle_rectangle.size.width = rectangle.size.width - 30;


    ei_color_t color_top;
    color_top.red = 96;
    color_top.green = 96;
    color_top.blue = 96;
    color_top.alpha = 0;

    ei_color_t color_middle;
    color_middle.red = 120;
    color_middle.green = 120;
    color_middle.blue = 120;
    color_middle.alpha = 0;

    ei_color_t color_bottom;
    color_bottom.red = 159;
    color_bottom.green = 159;
    color_bottom.blue = 159;
    color_bottom.alpha = 0;

    int area1 = 0;
    int area2 = 1;
    int area3 = 2;



    ei_point_t * list = list_of_points_for_a_rounded_frame(rectangle, radius1,&length1, area1);

    ei_point_t * list2 = list_of_points_for_a_rounded_frame(rectangle, radius1,&length2, area2);

    ei_point_t * list3 = list_of_points_for_a_rounded_frame(middle_rectangle, radius2,&length3, area3);

    ei_draw_polygon(surface, list, length1, color_bottom, NULL);
    ei_draw_polygon(surface, list2, length2, color_top, NULL);

    ei_draw_polygon(surface, list3, length3, color_middle, NULL);



}
