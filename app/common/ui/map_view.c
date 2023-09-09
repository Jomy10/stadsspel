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

void renderStreets(Olivec_Canvas* canvas, int renX, int renY, int renW, int renH, float zoomLevel, const MapRenderObjects *objs, const struct hashmap *mapnodes);
void renderStreet(Olivec_Canvas* canvas, const arPoint *points, int pointsCount, const char *kind);

void renderMapView(arView* self, Olivec_Canvas* canvas, arRect bounds) {
  const MapViewData* data = self->data;

  renderStreets(canvas, bounds.x, bounds.y, bounds.w, bounds.h, data->zoomLevel, *data->objs, *data->mapnodes);
}

arView* createMapView(MapViewData* data) {
  arView* view = al_alloc(arAllocator, sizeof(arView));
  view->subViews = NULL;
  view->data = data;
  view->render = &renderMapView;
  return view;
}

void renderStreets(Olivec_Canvas* canvas, int renX, int renY, int renW, int renH, float zoomLevel, const MapRenderObjects *objs, const struct hashmap *mapnodes) {
  if (objs == null || mapnodes == null) {
    printf("couldn't load map view %p %p\n", objs, mapnodes);
    olivec_fill(*canvas, 0xFF000bFF);
    return;
  }
  
  printf("[renderStreets] Drawing map view in %i %i %i %i\n", renX, renY, renW, renH);
  // SDL_SetRenderDrawColor(gRenderer, CMAP_MAIN);
  Vec(arPoint) points = createVec(arPoint, 100);
  arPoint point;
  u64 *nodeids;
  size_t nodeidsCount;
  MapNode *node;

  RO_Street *streets;
  int streetsSize;

  RO_getStreets(objs, &streets, &streetsSize);

  for (int streetIdx = 0; streetIdx < streetsSize; streetIdx++) {
    nodeids = streets[streetIdx].nodes;
    nodeidsCount = streets[streetIdx].nodes_count;

    for (size_t nodeIdx = 0; nodeIdx < nodeidsCount; nodeIdx++) {
      node = (MapNode *)hashmap_get((struct hashmap *)mapnodes, &(MapNode){.id = nodeids[nodeIdx]});
      // node = hasmap_get(objs, nodeids[nodeIdx]);
      if (node == null) {
        err("Node not found %llu\n", nodeids[nodeIdx]);
        continue;
      }

      assert(node->x >= 0 && node->x <= 1);
      assert(node->y >= 0 && node->y <= 1);

      point = (arPoint){
          .x = renX + node->x * renW * zoomLevel,
          .y = renY + node->y * renH * zoomLevel,
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
  // gDrawLines(gRenderer, points, pointsCount, null);
  for (int i = 1; i < pointsCount; i++) {
    olivec_line(*canvas, points[i].x, points[i].y, points[i-1].x, points[i-1].y, CMAP_MAIN);
  }
}

static void renderMapViewNavItem(struct _arView* self, Olivec_Canvas* canvas, arRect bounds) {
  olivec_rect(*canvas, bounds.x, bounds.y, bounds.w, bounds.h, 0xFF00FF00);
}

arView* createMapViewNavItem(void) {
  arView* view = al_alloc(arAllocator, sizeof(arView));
  view->subViews = NULL;
  view->data = NULL;
  view->render = &renderMapViewNavItem;
  return view;
}
