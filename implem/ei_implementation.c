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
    int *TC_length = calloc(2, sizeof(int));
    TC_length[0] = point_array->y;
    TC_length[1] = point_array->y;
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
    segment *curr_line;
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
            p_curr_seg->next = 0;
        }
        else
        {
            y_min = point_array[i + 1].y;
            p_curr_seg->y_max = point_array[i].y;
            p_curr_seg->x_y_min = point_array[i + 1].x;
            p_curr_seg->dx = point_array[i].x - point_array[i + 1].x;
            p_curr_seg->dy = point_array[i].y - point_array[i + 1].y;
            p_curr_seg->e = 0;
            p_curr_seg->next = 0;
        }
        /* We ignore horizontal lines */
        if (p_curr_seg->dy != 0)
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
                    curr_line = curr_line->next;
                }
                curr_line->next = p_curr_seg;
            }
        }
        else
        {
            free(p_curr_seg);
        }
    }
}

void ei_TCA_remove_merge(segment **TC, segment **p_TCA, uint16_t scanline, int TC_min)
{
    if (*p_TCA == NULL)
    {
        *p_TCA = TC[scanline];
    }
    else
    {
        segment *p_prev_seg = *p_TCA;
        segment *p_curr_seg = p_prev_seg->next;
        /* There is only one segment*/
        if (p_curr_seg == NULL)
        {
            if (p_curr_seg->y_max == scanline+TC_min)
            {
                *p_TCA = TC[scanline];
                free(p_curr_seg);
            }
            else
            {
                p_curr_seg->next = TC[scanline];
            }
        }
        /* There are at least 2 segments */
        else
        {
            while (p_curr_seg->next != 0)
            {
                if (p_curr_seg->y_max == scanline+TC_min)
                {
                    /* We remove the segment */
                    p_prev_seg->next = p_curr_seg->next;
                    free(p_curr_seg);
                    p_curr_seg = p_prev_seg->next;
                }
                else
                {
                    p_prev_seg = p_curr_seg;
                    p_curr_seg = p_curr_seg->next;
                }
            }
            /* Now, there is one last segment to check */
            if (p_curr_seg->y_max == scanline+TC_min)
            {
                p_prev_seg->next = TC[scanline];
                free(p_curr_seg);
            }
            else
            {
                p_curr_seg->next = TC[scanline];
            }
            /* This was never tested. */
            if ((*p_TCA)->y_max == scanline+TC_min)
            {
                segment *p_to_free_seg = *p_TCA;
                (*p_TCA) = (*p_TCA)->next;
                free(p_to_free_seg);
            }
        }
    }
}

void ei_draw_scanline(segment *TCA, ei_surface_t surface, ei_color_t color, int *TC_length, int line_idx)
{
    uint32_t pixel_color = ei_impl_map_rgba(surface, color);
    int width = hw_surface_get_size(surface).width;
    int offset = width * line_idx;
    uint32_t *p_first_pixel = (uint32_t *)hw_surface_get_buffer(surface) + offset; /* We set p_first_pixel to the first pixel of the scanline */

    segment *p_interval_entry = TCA;
    segment *p_interval_ending;
    uint16_t interval_entry_idx;
    uint16_t interval_ending_idx;
    /* The number of segments in TCA must be even since we removed horizontal and ending segments */
    while (p_interval_entry != NULL)
    {
        p_interval_ending = p_interval_entry->next;
        /* We round to the next number in entry. So we add 1 to x_y_min if epsilon > 0 */
        /* We round to the previous number in entry. So we substract 1 from x_y_min if epsilon <= 0 */
        if (p_interval_entry->dx < 0)
        {
            interval_entry_idx = p_interval_entry->x_y_min + (p_interval_entry->e < 0);
        }
        else
        {
            interval_entry_idx = p_interval_entry->x_y_min + (p_interval_entry->e > 0);  
        }
        if (p_interval_ending->dx < 0)
        {
            interval_ending_idx = p_interval_ending->x_y_min - (p_interval_ending->e >= 0);
        }
        else
        {
            interval_ending_idx = p_interval_ending->x_y_min - (p_interval_ending->e <= 0);
        }
        printf("e: %d, s: %d\n", interval_entry_idx, interval_ending_idx);
        for (uint32_t i = interval_entry_idx; i <= interval_ending_idx; i++)
        {
            *(p_first_pixel + i) = pixel_color;
        }
        p_interval_entry = p_interval_ending->next;
    }
}

void ei_update(segment *TCA)
{
    segment *p_curr_segment = TCA;
    while (p_curr_segment != NULL)
    {
        if (p_curr_segment->dx > 0)
        {
            p_curr_segment->e += p_curr_segment->dx;
            /* if |dx| > dy there may be several loop needed */
            while (p_curr_segment->e << 1 > p_curr_segment->dy)
            {
                p_curr_segment->x_y_min++;
                p_curr_segment->e -= p_curr_segment->dy;
            }
        }
        else if (p_curr_segment->dx < 0)
        {
            p_curr_segment->e -= p_curr_segment->dx;
            /* if |dx| > dy there may be several loop needed */
            while (p_curr_segment->e << 1 > p_curr_segment->dy)
            {
                p_curr_segment->x_y_min--;
                p_curr_segment->e -= p_curr_segment->dy;
            }
        }
        p_curr_segment = p_curr_segment->next;
    }
}

void ei_TCA_free(segment *TCA)
{
    if(TCA==NULL)
    {
        return;
    }
    else
    {
        segment *p_curr_segment=TCA->next;
        segment *p_prev_segment=TCA;
        while(p_curr_segment!=NULL)
        {
            free(p_prev_segment);
            p_prev_segment=p_curr_segment;
            p_curr_segment=p_curr_segment->next;
        }
        free(p_prev_segment);
    }
}

segment *ei_get_middle(segment *first)
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

segment *ei_TCA_sort(segment *first)
{
    if (first->next == NULL)
    {
        return first;
    }
    segment *middle = ei_get_middle(first);
    segment *next_to_middle = middle->next;
    middle->next = NULL;

    /* Apply merge on the left side */
    segment *first_part = ei_TCA_sort(first);

    /* Apply merge on the right side */
    segment *second_part = ei_TCA_sort(next_to_middle);

    /* merge both sides */
    segment *sorted = ei_merge(first_part, second_part);
    return sorted;
}

segment *ei_merge(segment *first, segment *second)
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
        result->next = ei_merge(first->next, second);
    }
    else
    {
        result = second;
        result->next = ei_merge(first, second->next);
    }
    return result;
}

void ei_list_print(segment *first)
{
    if (first == NULL)
    {
        return;
    }
    fprintf(stdout, "%i ", first->x_y_min);
    ei_list_print(first->next);
}