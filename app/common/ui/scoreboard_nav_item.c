//
//  scoreboard_nav_item.c
//  Stadsspel
//
//  Created by Jonas Everaert on 09/09/2023.
//

#include "include/scoreboard_nav_item.h"
#define OLIVEC_IMPLEMENTATION
#include <olive.c>

static void renderScoreboardNavItem(arView* self, Olivec_Canvas* canvas, arRect bound) {
    olivec_rect(*canvas, bound.x, bound.y, bound.w, bound.h, 0xFFFF0b01);
}

arView* createScoreboardViewNavItem(void) {
    arView* view = al_alloc(arAllocator, sizeof(arView));
    view->data = NULL;
    view->subViews = NULL;
    view->render = &renderScoreboardNavItem;
    return view;
}
