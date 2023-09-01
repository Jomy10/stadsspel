#include "include/map_view.h"

#include <assert.h>
#include <map_data/render_objects.h>
#include <render_frontend/renderer.h>
#include <util/types.h>
//#include <util/log.h>
#include "include/colors.h"
#include "include/coordinate_transform.h"
#include <hashmap.h>
#include <mercator.h>
#include <stdio.h>
#include <vec/vec.h>
#define err printf

void renderStreets(Renderer gRenderer, int renX, int renY, int renW, int renH, const MapRenderObjects *objs, const struct hashmap *mapnodes);
void renderStreet(Renderer gRenderer, const GPoint *points, int pointsCount, const char *kind);

void renderMapView(Renderer gRenderer, int renX, int renY, int renW, int renH, const MapRenderObjects *objs, const struct hashmap *mapnodes) {
  assert(gRenderer.ptr != null);
  assert(renX >= 0);
  assert(renY >= 0);
  assert(renW >= 0);
  assert(renH >= 0);
  assert(objs != null);

  renderStreets(gRenderer, renX, renY, renW, renH, objs, mapnodes);
}

void renderStreets(Renderer gRenderer, int renX, int renY, int renW, int renH, const MapRenderObjects *objs, const struct hashmap *mapnodes) {
  // SDL_SetRenderDrawColor(gRenderer, CMAP_MAIN);
  gSetStrokeColor(gRenderer, (GColor){CMAP_MAIN});
  Vec(GPoint) points = createVec(GPoint, 100);
  GPoint point;
  u64 *nodeids;
  size_t nodeidsCount;
  MapNode *node;

  RO_Street *streets;
  int streetsSize;

  RO_getStreets(objs, &streets, &streetsSize);
  // printf("%i\n", streetsSize);

  // TODO: projections in separate function! (calculated once)
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

      point = (GPoint){
          .x = renX + node->x * renW,
          .y = renY + node->y * renH,
      };

      assert(!isnan(point.x));
      assert(!isnan(point.y));

      vec_append(points, &point);
    }

    printf("Rendering street ");
    for (int i = 0; i < points->size; i++) {
      printf(" %f, %f >", ((GPoint*)points->values)[i].x, ((GPoint*)points->values)[i].y);
    }
    printf("\n");
    renderStreet(gRenderer, (GPoint *)points->values, points->size, streets[streetIdx].type);
    vec_clear(points);
  }

  vec_free(points);
}

void renderStreet(Renderer gRenderer, const GPoint *points, int pointsCount, const char *kind) {
  gDrawLines(gRenderer, points, pointsCount);
}
