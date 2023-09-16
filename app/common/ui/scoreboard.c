//
//  scoreboard_nav_item.c
//  Stadsspel
//
//  Created by Jonas Everaert on 09/09/2023.
//

#include "include/scoreboard.h"

#define OLIVEC_IMPLEMENTATION
#include <olive.c>

static void renderScoreboardContent(arView* self, Olivec_Canvas* canvas, arRect bounds) {
    olivec_rect(*canvas, bounds.x, bounds.y, bounds.w, bounds.h, 0xFF0b0b06);
}

arView* createScoreboardViewContent(ScoreboardViewData* data) {
    arView* view = al_alloc(arAllocator, sizeof(arView));
    view->data = data;
    view->subViews = NULL;
    view->render = &renderScoreboardContent;
    return view;
}

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
