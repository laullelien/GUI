#include "ei_application.h"
#include "ei_types.h"
#include "../implem/ei_impl_event.h"


int main(int argc, char* argv[])
{
    ei_rect_t rect1 = {{10, 10}, {100, 100}};
    ei_rect_t rect2 = {{20, 20}, {200, 200}};
    ei_rect_t rect3 = {{30, 30}, {300, 300}};

    ei_app_invalidate_rect(&rect1);
    ei_app_invalidate_rect(&rect2);
    ei_app_invalidate_rect(&rect3);

    free_p_rect_cell();

	return 0;
}