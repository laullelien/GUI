#include <stdlib.h>
#include "ei_types.h"
#include "ei_event.h"
#include "ei_utils.h"
#include "hw_interface.h"
#include "ei_draw.h"
#include "../implem/ei_implementation.h"
#include "../implem/ei_draw_arc.h"
#include "../implem/ei_impl_widgetclass.h"

int main(int argc, char *argv[])
{
    ei_rect_t f_rec;
    f_rec.top_left = (ei_point_t){5, 0};
    f_rec.size.height = 10;
    f_rec.size.width = 10;
    ei_rect_t s_rec;
    s_rec.top_left = (ei_point_t){0, 5};
    s_rec.size.height = 10;
    s_rec.size.width = 10;
    ei_rect_t *inter = ei_intersect_clipper(&f_rec, &s_rec);
    if (inter != NULL)
    {
        printf("%i, %i, %i, %i", inter->top_left.x, inter->top_left.y, inter->size.width, inter->size.height);
    }
    else
    {
        printf("0");
    }
    return 0;
}