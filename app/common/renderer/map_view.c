#include <map_view.h>

#include <SDL3/SDL.h>
#include <map_data/render_objects.h>
#include <assert.h>
#include <util/types.h>
#include <util/log.h>
#include <vec/vec.h>
#include <mercator.h>
#include <hashmap.h>
#include <coordinate_transform.h>
#include <mercator.h>
#include <colors.h>

void renderStreets(SDL_Renderer* gRenderer, int renX, int renY, int renW, int renH, MapRenderObjects* objs, struct hashmap* mapnodes);
void renderStreet(SDL_Renderer* gRenderer, SDL_FPoint* points, int pointsCount, char* kind);

void renderMapView(SDL_Renderer* gRenderer, int renX, int renY, int renW, int renH, MapRenderObjects* objs, struct hashmap* mapnodes) {
  assert(gRenderer != null);
  assert(renX >= 0);
  assert(renY >= 0);
  assert(renW >= 0);
  assert(renH >= 0);
  assert(objs != null);

  renderStreets(gRenderer, renX, renY, renW, renH, objs, mapnodes);
}


void renderStreets(SDL_Renderer* gRenderer, int renX, int renY, int renW, int renH, MapRenderObjects* objs, struct hashmap* mapnodes) {
  SDL_SetRenderDrawColor(gRenderer, CMAP_MAIN);
  Vec(SDL_FPoint) points = createVec(SDL_FPoint, 100);
  SDL_FPoint point;
  u64* nodeids;
  size_t nodeidsCount;
  MapNode* node;

  RO_Street* streets;
  int streetsSize;

  RO_getStreets(objs, &streets, &streetsSize);
  // printf("%i\n", streetsSize);

  // TODO: projections in separate function! (calculated once)
  for (int streetIdx = 0; streetIdx < streetsSize; streetIdx++) {
    nodeids = streets[streetIdx].nodes;
    nodeidsCount = streets[streetIdx].nodes_count;

    for (size_t nodeIdx = 0; nodeIdx < nodeidsCount; nodeIdx++) {
      node = (MapNode*) hashmap_get(mapnodes, &(MapNode){.id=nodeids[nodeIdx]});
      //node = hasmap_get(objs, nodeids[nodeIdx]);
      if (node == null) {
        err(0, "Node not found %llu\n", nodeids[nodeIdx]);
        continue;
      }

      assert(node->x >= 0 && node->x <= 1);
      assert(node->y >= 0 && node->y <= 1);

      point = (SDL_FPoint) {
        .x = node->x * renW,
        .y = node->y * renH,
      };

      assert(!isnan(point.x));
      assert(!isnan(point.y));

      vec_append(points, &point);
    }

    renderStreet(gRenderer, (SDL_FPoint*) points->values, points->size, streets[streetIdx].type);
    vec_clear(points);
  }

  vec_free(points);
}

void renderStreet(SDL_Renderer* gRenderer, SDL_FPoint* points, int pointsCount, char* kind) {
  SDL_RenderDrawLinesF(gRenderer, points, pointsCount);
}
