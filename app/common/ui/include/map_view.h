#ifndef _MAP_VIEW_H
#define _MAP_VIEW_H

#include "view.h"
#include <map_data/render_objects.h>
#include <hashmap.h>

#undef OLIVEC_IMPLEMENTATION
#include <olive.c>

typedef struct {
  const MapRenderObjects** objs;
  const struct hashmap** mapnodes;
  float zoomLevel;
} MapViewData;

arView* createMapView(MapViewData* data);

arView* createMapViewNavItem(void);

#endif
