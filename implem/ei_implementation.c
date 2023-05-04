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