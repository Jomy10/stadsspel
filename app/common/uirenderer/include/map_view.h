#ifndef _MAP_VIEW_H
#define _MAP_VIEW_H

#include <map_data/render_objects.h>
#include <renderer/renderer.h>
#include <hashmap.h>

void renderMapView(Renderer gRenderer, int renX, int renY, int renW, int renH, float zoom, const MapRenderObjects* objs, const struct hashmap* mapnodes);

#endif
