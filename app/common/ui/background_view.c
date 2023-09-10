//
//  background_view.c
//  Stadsspel
//
//  Created by Jonas Everaert on 09/09/2023.
//

#include "background_view.h"
#include <assert.h>

#define OLIVEC_IMPLEMENTATION
#include <olive.c>

#include <stdio.h>

void renderBackgroundFill(arView* view, Olivec_Canvas* canvas, arRect rect) {
  BackgroundFillViewData* data = view->data;
  //printf("Rendering backoung with color %#08x\n", data->fillColor);
  
  olivec_rect(*canvas, rect.x, rect.y, rect.w, rect.h, data->fillColor);
  
  assert(view->subViews->size == 1);
  arView* subview = ((arView**)view->subViews->values)[0];
  subview->render(subview, canvas, rect);
}

arView* createBackgroundFillView(BackgroundFillViewData* data, arView* subview) {
  arView* view = al_alloc(arAllocator, sizeof(arView));
  view->data = data;
  view->subViews = createVecA(arView*, 1, arAllocator);
  ar_addSubView(view, subview);
  view->render = &renderBackgroundFill;
  return view;
}

