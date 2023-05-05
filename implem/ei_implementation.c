#include "ei_implementation.h"
#include <stdint.h>

uint32_t ei_impl_map_rgba(ei_surface_t surface, ei_color_t color)
{
    int ir, ig, ib, ia;
    hw_surface_get_channel_indices(surface, &ir, &ig, &ib, &ia);
    uint8_t pixel_color[4];
    pixel_color[ir] = color.red;
    pixel_color[ig] = color.green;
    pixel_color[ib] = color.blue;
    if (ia != -1)
    {
        pixel_color[ia] = color.alpha;
    }
    return *((uint32_t *)pixel_color);
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

void ei_TC_fill(segment **TC, ei_point_t *point_array, size_t point_array_size, int TC_min)
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

void ei_TCA_remove_merge(segment **TC, segment *TCA, uint16_t scanline)
{
    if (TCA == NULL)
    {
        TCA = TC[scanline];
    }
    /* if TCA is still NULL, we stop */
    if (TCA != NULL)
    {
        segment *p_prev_seg = TCA;
        segment *p_curr_seg = p_prev_seg->next;
        /* There is only one segment*/
        if (p_curr_seg == NULL)
        {
            if (p_curr_seg->y_max == scanline)
            {
                TCA = TC[scanline];
            }
            else
            {
                p_curr_seg->next = TC[scanline];
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
                    p_curr_seg = p_prev_seg->next;
                }
                else
                {
                    p_prev_seg = p_curr_seg;
                    p_curr_seg = p_curr_seg->next;
                }
            }
            /* Now, there is one last segment to check */
            if (p_curr_seg->y_max == scanline)
            {
                p_prev_seg->next = TC[scanline];
                free(p_curr_seg);
            }
            else
            {
                p_curr_seg->next = TC[scanline];
            }
        }
    }
    pixel_color[ia] = color.alpha;
}
return *((uint32_t *)pixel_color);
}

segment *get_middle(segment *first)
{
    if (first->next == NULL)
    {
        return first;
    }
    segment *slow = first;
    segment *fast = first;
    while (fast->next != NULL && fast->next->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

segment *linked_list_merge_sort(segment *first)
{
    if (first->next == NULL)
    {
        return first;
    }
    segment *middle = get_middle(first);
    segment *next_to_middle = middle->next;
    // fprintf(stdout, "%i %i ", middle->x_y_min, next_to_middle->x_y_min);
    middle->next = NULL;

    /* Apply merge on the left side */
    segment *first_part = linked_list_merge_sort(first);

    /* Apply merge on the right side */
    segment *second_part = linked_list_merge_sort(next_to_middle);

    /* merge both sides */
    segment *sorted = linked_list_sort(first_part, second_part);
    return sorted;
}

segment *linked_list_sort(segment *first, segment *second)
{
    segment *result = NULL;
    if (first == NULL)
    {
        return second;
    }
    if (second == NULL)
    {
        return first;
    }

    if (first->x_y_min <= second->x_y_min)
    {
        result = first;
        result->next = linked_list_sort(first->next, second);
    }
    else
    {
        result = second;
        result->next = linked_list_sort(first, second->next);
    }
    return result;
}

void print_list(segment *first)
{
    if (first == NULL)
    {
        return;
    }
    fprintf(stdout, "%i ", first->x_y_min);
    print_list(first->next);
}