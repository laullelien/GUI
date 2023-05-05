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


segment * get_middle(segment * first)
{
    if(first->next == NULL){return first;}
    segment * slow = first;
    segment * fast = first;
    while (fast->next != NULL && fast->next->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}


segment* linked_list_merge_sort(segment * first)
{
    if (first->next == NULL){return first;}
    segment * middle = get_middle(first);
    segment * next_to_middle = middle->next;
    //fprintf(stdout, "%i %i ", middle->x_y_min, next_to_middle->x_y_min);
    middle->next = NULL;

    /* Apply merge on the left side */
    segment * first_part = linked_list_merge_sort(first);


    /* Apply merge on the right side */
    segment * second_part = linked_list_merge_sort(next_to_middle);

    /* merge both sides */
    segment * sorted = linked_list_sort(first_part, second_part);
    return sorted;
}


segment* linked_list_sort(segment * first, segment * second)
{
    segment * result = NULL;
    if (first == NULL){return second;}
    if (second == NULL){return first;}

    if (first->x_y_min <= second->x_y_min)
    {
        result =  first;
        result->next = linked_list_sort(first->next, second);
    }
    else
    {
        result =  second;
        result->next = linked_list_sort(first, second->next);
    }
    return result;
}

void print_list(segment * first)
{
    if (first == NULL)
    {
        return;
    }
    fprintf(stdout, "%i ", first->x_y_min);
    print_list(first->next);
}