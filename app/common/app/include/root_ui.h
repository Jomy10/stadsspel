#ifndef _APP_DEFAULT_UI_H
#define _APP_DEFAULT_UI_H

#include <ui/view.h>
#include <ui/nav_view.h>
#include <map_data/render_objects.h>
#include <hashmap.h>

#undef OLIVEC_IMPLEMENTATION
#include <olive.c>

void createRootView(
    arView** root,
    float** mapZoomLevel, arPoint** mapZoomMid, arPoint** mapTranslation,
    int** navSize, int** selectedNav, NavViewData** navViewData,
    const MapRenderObjects* objs, const struct hashmap** mapnodes);

#endif
