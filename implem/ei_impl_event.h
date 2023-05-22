#ifndef PROJETC_IG_EI_IMPL_EVENT_H
#define PROJETC_IG_EI_IMPL_EVENT_H

#include "ei_types.h"


ei_widget_t get_widget_from_mouse_location(ei_event_t *event, ei_surface_t pick_surface);

void merge_rect_clipper(ei_linked_rect_t * rects);



#endif //PROJETC_IG_EI_IMPL_EVENT_H
