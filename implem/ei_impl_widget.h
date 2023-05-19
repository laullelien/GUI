#include "ei_widgetclass.h"

void ei_insert_child(ei_widget_t parent, ei_widget_t child);

void initialize_widget_list();

uint32_t get_current_picking_id();
void insert_widget(ei_widget_t widget, ei_widget_t **plist);

ei_widget_t **get_widget_list_pointer();
void ei_id_to_color(uint32_t color_id);
void ei_color_to_id(ei_color_t *color);
void ei_widget_destroy_rec(ei_widget_t widget);