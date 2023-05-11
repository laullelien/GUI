#include "../api/ei_placer.h"
#include "ei_implementation.h"

void ei_place(ei_widget_t widget,
              ei_anchor_t *anchor,
              int *x,
              int *y,
              int *width,
              int *height,
              float *rel_x,
              float *rel_y,
              float *rel_width,
              float *rel_height)
{
    widget->screen_location.top_left = (ei_point_t){*x, *y};
}
