//
//  nav_view.c
//  Stadsspel
//
//  Created by Jonas Everaert on 09/09/2023.
//

#include "include/nav_view.h"

#include <stdio.h>

#define OLIVEC_IMPLEMENTATION
#include <olive.c>

void renderNavView(struct _arView* self, Olivec_Canvas* canvas) {
  if (self->subViews == NULL) return;

  const NavViewData* data = self->data;

  arRect contentBounds = {0, 0, canvas->width, canvas->height - data->navSize};
  Olivec_Canvas contentCanvas = olivec_subcanvas(*canvas, contentBounds.x, contentBounds.y, contentBounds.w, contentBounds.h);

  arView* contentView = ((arView**)self->subViews->values)[data->selectedView];
  arView** navViews = data->navViews->values;

  //printf("Rendering nav view content %i at (%i %i %i %i)\n", data->selectedView, contentBounds.x, contentBounds.y, contentBounds.w, contentBounds.h);
  contentView->render(contentView, &contentCanvas);
  int navWidth = canvas->width / self->subViews->size;
  for (int i = 0; i < self->subViews->size; i++) {
    //printf("Rendering nav item (%i %i %i %i)\n", bounds.x + i * navWidth, bounds.y + contentBounds.h, navWidth, data->navSize);
    Olivec_Canvas subcanvas = olivec_subcanvas(*canvas, i * navWidth, contentBounds.h, navWidth, data->navSize);
    navViews[i]->render(navViews[i], &subcanvas);//, (arRect){bounds.x + i * navWidth, bounds.y + contentBounds.h, navWidth, data->navSize});
  }
}

arView* createNavView(NavViewData* data) {
  arView* view = al_alloc(arAllocator, sizeof(arView));
  view->subViews = createVecA(arView*, 0, arAllocator);
  view->data = data;
  view->render = &renderNavView;
  return view;
}

/// Returns the index of the nav view that was clicked by the user
int navview_touchedNavViewIndex(NavViewData* data, arRect viewBounds, arPoint pos) {
  printf("%i %i in %i %i %i %i\n", pos.x, pos.y, viewBounds.x, viewBounds.y, viewBounds.w, viewBounds.h);
  if (
       pos.x >= viewBounds.x && pos.x <= viewBounds.x + viewBounds.w
    && pos.y >= viewBounds.y + (viewBounds.h - data->navSize) && pos.y <= viewBounds.h
  ) {
    // point is inside of bottom nav bar
    int navWidth = viewBounds.w / data->navViews->size;
    for (int i = 0; i < data->navViews->size; i++) {
      if (pos.x >= viewBounds.x + i * navWidth && pos.x <= viewBounds.x + (i + 1) * navWidth) {
        return i;
      }
    }
  }

  return -1;
}
