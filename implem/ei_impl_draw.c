#include "ei_impl_draw.h"
#include "../api/ei_types.h"
#include "ei_implementation.h"
#include "ei_application.c"

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

void ei_TC_fill(ei_segment **TC, ei_point_t *point_array, size_t point_array_size, int TC_min)
{
    /* We store y_min to know in what line of TC we will ad it */
    int y_min;
    ei_segment *p_curr_seg;
    ei_segment *curr_line;
    for (size_t i = 0; i < point_array_size - 1; i++)
    {
        p_curr_seg = malloc(sizeof(ei_segment));
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

void ei_TCA_remove_merge(ei_segment **TC, ei_segment **p_TCA, uint16_t scanline, int TC_min)
{
    if (*p_TCA == NULL)
    {
        *p_TCA = TC[scanline];
    }
    else
    {
        ei_segment *p_prev_seg = *p_TCA;
        ei_segment *p_curr_seg = p_prev_seg->next;
        /* There is only one segment*/
        if (p_curr_seg == NULL)
        {
            if (p_curr_seg->y_max == scanline + TC_min)
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
                if (p_curr_seg->y_max == scanline + TC_min)
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
            if (p_curr_seg->y_max == scanline + TC_min)
            {
                p_prev_seg->next = TC[scanline];
                free(p_curr_seg);
            }
            else
            {
                p_curr_seg->next = TC[scanline];
            }
            /* This was never tested. */
            if ((*p_TCA)->y_max == scanline + TC_min)
            {
                ei_segment *p_to_free_seg = *p_TCA;
                (*p_TCA) = (*p_TCA)->next;
                free(p_to_free_seg);
            }
        }
    }
}

void ei_draw_scanline(ei_surface_t surface, ei_segment *TCA, const ei_rect_t *clipper, uint32_t pixel_color, ei_color_t color, int width, int line_idx)
{

    int offset = width * line_idx;
    uint32_t *p_first_pixel = (uint32_t *)hw_surface_get_buffer(surface) + offset; /* We set p_first_pixel to the first pixel of the scanline */

    ei_segment *p_interval_entry = TCA;
    ei_segment *p_interval_ending;
    int interval_entry_idx;
    int interval_ending_idx;
    /* The number of segments in TCA must be even since we removed horizontal and ending segments */
    while (p_interval_entry != NULL)
    {
        p_interval_ending = p_interval_entry->next;
        /* We round to the next number in entry. So we add 1 to x_y_min if and only if epsilon > 0 */
        /* We round to the previous number in entry. So we substract 1 from x_y_min if and only if epsilon <= 0 */
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
        // printf("e: %d, s: %d\n", interval_entry_idx, interval_ending_idx);
        if (interval_entry_idx < clipper->top_left.x)
        {
            interval_entry_idx = clipper->top_left.x;
        }
        if (interval_ending_idx > clipper->top_left.x + clipper->size.width - 1)
        {
            interval_ending_idx = clipper->top_left.x + clipper->size.width - 1;
        }
        if (color.alpha == 255 || surface == ei_get_picking_surface())
        {
            for (int i = interval_entry_idx; i <= interval_ending_idx; i++)
            {
                *(p_first_pixel + i) = pixel_color;
            }
        }
        else
        {
            int ir = ei_get_ir();
            int ig = ei_get_ig();
            int ib = ei_get_ib();
            int ia = ei_get_ia();
            int src_alpha = color.alpha;
            int src_alpha_comp = 255 - src_alpha;

            p_first_pixel += interval_entry_idx;
            for (int i = 0; i <= interval_ending_idx - interval_entry_idx; i++)
            {
                *((uint8_t *)p_first_pixel + ia) = 255;
                *((uint8_t *)p_first_pixel + ir) = (uint8_t)(((uint16_t)(*((uint8_t *)p_first_pixel + ir)) * (src_alpha_comp) + (uint16_t)(color.red) * src_alpha) / 255);
                *((uint8_t *)p_first_pixel + ig) = (uint8_t)(((uint16_t)(*((uint8_t *)p_first_pixel + ig)) * (src_alpha_comp) + (uint16_t)(color.green) * src_alpha) / 255);
                *((uint8_t *)p_first_pixel + ib) = (uint8_t)(((uint16_t)(*((uint8_t *)p_first_pixel + ib)) * (src_alpha_comp) + (uint16_t)(color.blue) * src_alpha) / 255);
                p_first_pixel++;
            }
        }
        p_interval_entry = p_interval_ending->next;
    }
}

void ei_update(ei_segment *TCA)
{
    ei_segment *p_curr_segment = TCA;
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

void ei_TCA_free(ei_segment *TCA)
{
    if (TCA == NULL)
    {
        return;
    }
    else
    {
        ei_segment *p_curr_segment = TCA->next;
        ei_segment *p_prev_segment = TCA;
        while (p_curr_segment != NULL)
        {
            free(p_prev_segment);
            p_prev_segment = p_curr_segment;
            p_curr_segment = p_curr_segment->next;
        }
        free(p_prev_segment);
    }
}

ei_segment *ei_get_middle(ei_segment *first)
{
    if (first->next == NULL)
    {
        return first;
    }
    ei_segment *slow = first;
    ei_segment *fast = first;
    while (fast->next != NULL && fast->next->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}
ei_segment *ei_merge(ei_segment *first, ei_segment *second)
{
    ei_segment *result = NULL;
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

ei_segment *ei_TCA_sort(ei_segment *first)
{
    if (first->next == NULL)
    {
        return first;
    }
    ei_segment *middle = ei_get_middle(first);
    ei_segment *next_to_middle = middle->next;
    middle->next = NULL;

    /* Apply merge on the left side */
    ei_segment *first_part = ei_TCA_sort(first);

    /* Apply merge on the right side */
    ei_segment *second_part = ei_TCA_sort(next_to_middle);

    /* merge both sides */
    ei_segment *sorted = ei_merge(first_part, second_part);
    return sorted;
}

void ei_list_print(ei_segment *first)
{
    if (first == NULL)
    {
        return;
    }
    fprintf(stdout, "%i ", first->x_y_min);
    ei_list_print(first->next);
}

void ei_initialize_borders(const ei_rect_t *clipper,
                           ei_borders *borders)
{
    borders->left = (clipper->top_left).x;
    borders->right = (clipper->top_left).x + (clipper->size).width;
    borders->upper = (clipper->top_left).y;
    borders->lower = (clipper->top_left).y + (clipper->size).height;
}

bool ei_inside_clipper(ei_point_t *point,
                       const ei_rect_t *clipper,
                       ei_borders *borders)
{
    return (clipper == 0) || ((point->x >= borders->left) && (point->x <= borders->right) && (point->y >= borders->upper) && (point->y <= borders->lower));
}

bool ei_intersect_line_clipper(ei_point_t *first_point, ei_point_t *last_point, const ei_rect_t *clipper, int dx, int dy, int *e)
{
    uint8_t first_code = ((first_point->y > clipper->top_left.y + clipper->size.height - 1) << 3) + ((first_point->y < clipper->top_left.y) << 2) + ((first_point->x > clipper->top_left.x + clipper->size.width - 1) << 1) + ((first_point->x < clipper->top_left.x));
    uint8_t last_code = ((last_point->y > clipper->top_left.y + clipper->size.height - 1) << 3) + ((last_point->y < clipper->top_left.y) << 2) + ((last_point->x > clipper->top_left.x + clipper->size.width - 1) << 1) + ((last_point->x < clipper->top_left.x));
    bool in = (first_code | last_code) == 0;
    bool out = (first_code & last_code) != 0;
    if (in)
    {
        return true;
    }
    if (out)
    {
        return false;
    }
    if (first_code != 0)
    {
        /* x_1 < x_min*/
        if (first_code & 1)
        {
            double y = (double)(dy * (clipper->top_left.x - first_point->x)) / (double)dx;
            if (dx > abs(dy))
            {
                *e = (clipper->top_left.x - first_point->x) * abs(dy) % dx;
                if (*e << 1 > dx)
                {
                    *e -= dx;
                }
            }
            else
            {
                *e = abs(first_point->y - (int)round(y)) * dx % abs(dy);
                if ((*e << 1) > abs(dy))
                {
                    *e -= abs(dy);
                }
            }
            first_point->y += (int)round(y);
            first_point->x = clipper->top_left.x;
            return ei_intersect_line_clipper(first_point, last_point, clipper, dx, dy, e);
        }

        /* x_1 > x_max*/
        else if (first_code & 2)
        {
            double y = (double)(dy * (first_point->x - clipper->top_left.x - clipper->size.width + 1)) / (double)(-dx);
            if ((-dx) > abs(dy))
            {
                *e = (first_point->x - clipper->top_left.x - clipper->size.width + 1) * abs(dy) % (-dx);
                if (*e << 1 > (-dx))
                {
                    *e += dx;
                }
            }
            else
            {
                *e = abs(first_point->y - (int)round(y)) * (-dx) % abs(dy);
                if (*e << 1 > abs(dy))
                {
                    *e -= abs(dy);
                }
            }
            first_point->y += (int)round(y);
            first_point->x = clipper->top_left.x + clipper->size.width - 1;
            return ei_intersect_line_clipper(first_point, last_point, clipper, dx, dy, e);
        }

        /* y_1 < y_min*/
        else if (first_code & 4)
        {
            double x = (double)(dx * (clipper->top_left.y - first_point->y)) / (double)dy;
            if (dy > abs(dx))
            {
                *e = (clipper->top_left.y - first_point->y) * abs(dx) % dy;
                if (*e << 1 > dy)
                {
                    *e -= dy;
                }
            }
            else
            {
                *e = abs(first_point->x - (int)round(x)) * dy % abs(dx);
                if (*e << 1 > abs(dx))
                {
                    *e -= abs(dx);
                }
            }
            first_point->x += (int)round(x);
            first_point->y = clipper->top_left.y;
            return ei_intersect_line_clipper(first_point, last_point, clipper, dx, dy, e);
        }

        /* y_1 > y_max*/
        else
        {
            double x = (double)(dx * (first_point->y - clipper->top_left.y - clipper->size.height + 1)) / (double)(-dy);
            if ((-dy) > abs(dx))
            {
                *e = (first_point->y - clipper->top_left.y - clipper->size.width + 1) * abs(dx) % (-dy);
                if (*e << 1 > (-dy))
                {
                    *e += dy;
                }
            }
            else
            {
                *e = abs(first_point->x - (int)round(x)) * (-dy) % abs(dx);
                if (*e << 1 > abs(dx))
                {
                    *e -= abs(dx);
                }
            }
            first_point->x += (int)round(x);
            first_point->y = clipper->top_left.y + clipper->size.height - 1;
            return ei_intersect_line_clipper(first_point, last_point, clipper, dx, dy, e);
        }
    }
    else
    {
        /* x_2 < x_min*/
        if (last_code & 1)
        {
            double y = (double)(dy * (clipper->top_left.x - last_point->x)) / (double)dx;
            last_point->y += (int)round(y);
            last_point->x = clipper->top_left.x;
            return ei_intersect_line_clipper(first_point, last_point, clipper, dx, dy, e);
        }

        /* x_2 > x_max*/
        else if (last_code & 2)
        {
            double y = (double)(dy * (last_point->x - clipper->top_left.x - clipper->size.width + 1)) / (double)(-dx);
            last_point->y += (int)round(y);
            last_point->x = clipper->top_left.x + clipper->size.width - 1;
            return ei_intersect_line_clipper(first_point, last_point, clipper, dx, dy, e);
        }

        /* y_2 < y_min*/
        else if (last_code & 4)
        {
            double x = (double)(dx * (clipper->top_left.y - last_point->y)) / (double)dy;
            last_point->x += (int)round(x);
            last_point->y = clipper->top_left.y;
            return ei_intersect_line_clipper(first_point, last_point, clipper, dx, dy, e);
        }

        /* y_2 > y_max*/
        else
        {
            double x = (double)(dx * (last_point->y - clipper->top_left.y - clipper->size.height + 1)) / (double)(-dy);
            last_point->x += (int)round(x);
            last_point->y = clipper->top_left.y + clipper->size.height - 1;
            return ei_intersect_line_clipper(first_point, last_point, clipper, dx, dy, e);
        }
    }
}

void ei_TC_free(ei_segment **TC, int length, int first_unused_TC_line)
{
    for (int line_idx = first_unused_TC_line; line_idx < length; line_idx++)
    {
        ei_segment *p_prev_seg = TC[line_idx];
        if (p_prev_seg == NULL)
        {
            continue;
        }
        ei_segment *p_curr_seg = p_prev_seg->next;
        while (p_curr_seg != NULL)
        {
            free(p_prev_seg);
            p_prev_seg = p_curr_seg;
            p_curr_seg = p_prev_seg->next;
        }
        free(p_prev_seg);
    }
}

int min(int a, int b)
{
    return (a < b) ? a : b;
}

ei_rect_t *ei_intersect_clipper(ei_rect_t *first_clipper, ei_rect_t *second_clipper)
{
    ei_rect_t *intersection = malloc(sizeof(ei_rect_t));

    /* defines topleft.x and width */
    if (first_clipper->top_left.x < second_clipper->top_left.x)
    {
        if (first_clipper->top_left.x + first_clipper->size.width <= second_clipper->top_left.x)
        {
            free(intersection);
            return NULL;
        }
        else
        {
            intersection->top_left.x = second_clipper->top_left.x;
            intersection->size.width = min(first_clipper->top_left.x + first_clipper->size.width - second_clipper->top_left.x, second_clipper->size.width);
        }
    }
    else
    {
        if (second_clipper->top_left.x + second_clipper->size.width <= first_clipper->top_left.x)
        {
            free(intersection);
            return NULL;
        }
        else
        {
            intersection->top_left.x = first_clipper->top_left.x;
            intersection->size.width = min(second_clipper->top_left.x + second_clipper->size.width - first_clipper->top_left.x, first_clipper->size.width);
        }
    }

    /* defines topleft.y and height */
    if (first_clipper->top_left.y <= second_clipper->top_left.y)
    {
        if (first_clipper->top_left.y + first_clipper->size.height < second_clipper->top_left.y)
        {
            free(intersection);
            return NULL;
        }
        else
        {
            intersection->top_left.y = second_clipper->top_left.y;
            intersection->size.height = min(first_clipper->top_left.y + first_clipper->size.height - second_clipper->top_left.y, second_clipper->size.height);
        }
    }
    else
    {
        if (second_clipper->top_left.y + second_clipper->size.height <= first_clipper->top_left.y)
        {
            free(intersection);
            return NULL;
        }
        else
        {
            intersection->top_left.y = first_clipper->top_left.y;
            intersection->size.height = min(second_clipper->top_left.y + second_clipper->size.height - first_clipper->top_left.y, first_clipper->size.height);
        }
    }

    /* intersection is null */
    if (intersection->size.height == 0 || intersection->size.width == 0)
    {
        free(intersection);
        return NULL;
    }
    return intersection;
}
