#include "include/view.h"
#include <string.h>

Allocator* arAllocator;

bool recteql(arRect* rect1, arRect* rect2) {
  return memcmp(rect1, rect2, sizeof(arRect)) == 0;
}

void renderArView(arView* self, Olivec_Canvas* canvas) {
  arView* view;
  for (int i = 0; i < self->subViews->size; i++) {
    view = ((arView**)(self->subViews->values))[i];
    view->render(view, canvas);
  }
}

arView* createArView(void) {
  arView* view = al_alloc(arAllocator, sizeof(arView));
  view->subViews = createVecA(arView*, 0, arAllocator);
  view->data = NULL;
  view->render = &renderArView;
  return view;
}

void freeArView(arView* view) {
  if (view->subViews != NULL) {
    for (int i = 0; i < view->subViews->size; i++) {
      freeArView(((arView**)view->subViews->values)[i]);
    }
    veca_free(view->subViews);
  }
  al_free(arAllocator, view);
}

void ar_addSubView(arView* root, arView* view) {
  if (root->subViews == NULL) {
    root->subViews = createVecA(arView*, 1, arAllocator);
  }
  veca_append(root->subViews, &view);
}

void ar_setAllocator(Allocator* al) {
  arAllocator = al;
}
