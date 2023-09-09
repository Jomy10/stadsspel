//
//  nav_view.h
//  Stadsspel
//
//  Created by Jonas Everaert on 09/09/2023.
//

#ifndef nav_view_h
#define nav_view_h

#include "view.h"

typedef struct {
    VecA(arView*) navViews;
    int selectedView;
    int navSize;
} NavViewData;

arView* createNavView(NavViewData* data);

int touchedNavViewIndex(NavViewData* data, arRect viewBounds, arPoint pos);

#endif /* nav_view_h */
