#include "include/renderer.h"
#include "include/map_view.h"
#include <render_frontend/renderer.h>
#include <map_data/render_objects.h>

#include <stdio.h>

void renderApp(Renderer ren, MapRenderObjects* objs, struct hashmap* mapnodes, GRect screenBounds) {

  renderMapView(ren, screenBounds.x, screenBounds.y, screenBounds.w, screenBounds.h, objs, mapnodes);

  gRenderUpdate(ren);
}
