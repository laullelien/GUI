#include "ei_widgetclass.h"

void ei_insert_child(ei_widget_t parent, ei_widget_t child);

void initialize_widget_list();

uint32_t get_current_picking_id();
void insert_widget(ei_widget_t widget, ei_widget_t ** plist);

ei_widget_t ** get_widget_list_pointer();
