#include "ei_types.h"


static ei_linked_rect_t* p_rect_cell = NULL;


ei_linked_rect_t** get_pp_rect_cell()
{
    return &p_rect_cell;
}

ei_linked_rect_t* get_p_rect_cell()
{
    return p_rect_cell;
}

void free_p_rect_cell()
{
    ei_linked_rect_t* current = p_rect_cell;
    while (p_rect_cell != NULL)
    {
        p_rect_cell = p_rect_cell->next;
        free(current);
        current = p_rect_cell;
    }
    p_rect_cell = NULL;
}