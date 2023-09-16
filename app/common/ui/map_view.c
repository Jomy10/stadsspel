#include "include/map_view.h"
#include "include/color.h"
#include "include/coordinate_transform.h"
#include "include/view.h"
#include <vec/vec.h>
#include <map_data/render_objects.h>
#include <hashmap.h>
#include <mercator.h>
#include <util/types.h>
#include <stdio.h>
#include <assert.h>

#define OLIVEC_IMPLEMENTATION
#include <olive.c>

#define err printf

void renderStreets(Olivec_Canvas* canvas, int renX, int renY, int renW, int renH, float zoomLevel, arPoint zoomMid, const MapRenderObjects *objs, const struct hashmap *mapnodes);
void renderStreet(Olivec_Canvas* canvas, const arPoint *points, int pointsCount, const char *kind);

void renderMapView(arView* self, Olivec_Canvas* canvas, arRect bounds) {
  const MapViewData* data = self->data;

  renderStreets(canvas, bounds.x, bounds.y, bounds.w, bounds.h, data->zoomLevel, data->mapScaleMid, data->objs, *data->mapnodes);
}

arView* createMapView(MapViewData* data) {
  arView* view = al_alloc(arAllocator, sizeof(arView));
  view->subViews = NULL;
  view->data = data;
  view->render = &renderMapView;
  return view;
}

void renderStreets(Olivec_Canvas* canvas, int renX, int renY, int renW, int renH, float zoomLevel, arPoint zoomMid, const MapRenderObjects *objs, const struct hashmap *mapnodes) {
  if (objs == null || mapnodes == null) {
    printf("couldn't load map view %p %p\n", objs, mapnodes);
    olivec_fill(*canvas, 0xFF000bFF);
    return;
  }

  arRect frame = {renX, renY, renW, renH};
  frame.w *= zoomLevel;
  frame.h *= zoomLevel;
  frame.x += (renW - frame.w) / 2 + ((renW / 2) - (zoomMid.x == -1 ? (renW / 2) : zoomMid.x));
  frame.y += (renH - frame.h) / 2 + ((renH / 2) - (zoomMid.y == -1 ? (renH / 2) : zoomMid.y));

//  printf("[renderStreets] Drawing map view in %i %i %i %i\n", renX, renY, renW, renH);
  Vec(arPoint) points = createVec(arPoint, 100);
  arPoint point;
  u64* nodeids;
  size_t nodeidsCount;
  MapNode* node;

  RO_Street* streets;
  int streetsSize;

  RO_getStreets(objs, &streets, &streetsSize);

  for (int streetIdx = 0; streetIdx < streetsSize; streetIdx++) {
    nodeids = streets[streetIdx].nodes;
    nodeidsCount = streets[streetIdx].nodes_count;

    for (size_t nodeIdx = 0; nodeIdx < nodeidsCount; nodeIdx++) {
      node = (MapNode *)hashmap_get((struct hashmap *)mapnodes, &(MapNode){.id = nodeids[nodeIdx]});
      if (node == null) {
        err("Node not found %llu\n", nodeids[nodeIdx]);
        continue;
      }

      assert(node->x >= 0 && node->x <= 1);
      assert(node->y >= 0 && node->y <= 1);

      point = (arPoint){
          .x = frame.x /*- (frame.x - renX) */+ node->x * frame.w, // renW * zoomLevel,
          .y = frame.y /*- (frame.y - renY) */+ node->y * frame.h, // renH * zoomLevel,
      };

      assert(!isnan(point.x));
      assert(!isnan(point.y));

      vec_append(points, &point);
    }

    renderStreet(canvas, (arPoint *)points->values, points->size, streets[streetIdx].type);
    vec_clear(points);
  }

  vec_free(points);
}

void renderStreet(Olivec_Canvas* canvas, const arPoint *points, int pointsCount, __attribute__((unused)) const char *kind) {
  for (int i = 1; i < pointsCount; i++) {
    olivec_line(*canvas, points[i].x, points[i].y, points[i-1].x, points[i-1].y, CMAP_MAIN);
  }
}

static void renderMapViewNavItem(__attribute__((unused)) struct _arView* self, Olivec_Canvas* canvas, arRect bounds) {
  olivec_rect(*canvas, bounds.x, bounds.y, bounds.w, bounds.h, 0xFF00FF00);
}

arView* createMapViewNavItem(void) {
  arView* view = al_alloc(arAllocator, sizeof(arView));
  view->subViews = NULL;
  view->data = NULL;
  view->render = &renderMapViewNavItem;
  return view;
}
