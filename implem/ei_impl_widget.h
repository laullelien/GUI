/**
 * @file	ei_impl_widget.h
 *
 * @brief 	Implements functions to initialise and destroy widgets.
 *
 */


#ifndef EI_IMPL_WIDGET_H
#define EI_IMPL_WIDGET_H


#include "ei_widgetclass.h"

/**
 * \brief	initializes the widget list that will contain widgets
 *          and the current picking id to distinguish widgets.
 */
void initialize_widget_list();


/**
 * \brief   a getter function to get the widget list
 *
 * @return 	a pointer to the widget list
 */
ei_widget_t **get_widget_list_pointer();

/**
 * \brief   inserts a widget into the widget list. If the length is
 *          not large enough, the length is extended.
 *
 * @param   widget  the widget to add
 * @param   plist   the widget list that contain all widgets
 */
void insert_widget(ei_widget_t widget, ei_widget_t **plist);


/**
 * \brief   inserts a child to a parent
 *
 * @param   parent  the widget to add
 * @param   child   the widget list that contain all widgets
 */
void ei_insert_child(ei_widget_t parent, ei_widget_t child);

/**
 * \brief   releases widget by destroying all the children of the widget
 *
 * @param   widget  the widget to destroy
 */
void ei_widget_destroy_rec(ei_widget_t widget);


#endif
