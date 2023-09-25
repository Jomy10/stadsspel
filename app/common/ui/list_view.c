//
//  list_view.c
//  Stadsspel
//
//  Created by Jonas Everaert on 09/09/2023.
//

#include "list_view.h"
#include <util/tagged_union.h>

#define OLIVEC_IMPLEMENTATION
#include <olive.c>

extern Allocator* arAllocator;

static void renderUniformListView(arView* self, Olivec_Canvas* canvas) {
    UniformListViewData* data = self->data;

    arView** subviews = self->subViews->values;
    int y = 0;

    for (int i = 0; i < self->subViews->size; i++) {
        Olivec_Canvas subcanvas = olivec_subcanvas(*canvas, 0, y, canvas->width, data->subviewHeight);
        subviews[i]->render(subviews[i], &subcanvas);
                            // (arRect){bounds.x, y, bounds.w, data->subviewHeight});
        y += data->subviewHeight + data->itemMargin;
    }
}

arView* createUniformListView(UniformListViewData* data) {
    arView* view = al_alloc(arAllocator, sizeof(arView));
    view->data = data;
    view->subViews = createVecA(arView*, 0, arAllocator);
    view->render = &renderUniformListView;
    return view;
}
