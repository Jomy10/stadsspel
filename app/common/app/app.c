#include <hashmap.h>
#include <stdbool.h>
#include <map_data/render_objects.h>
#include <map_data/parse.h>
#include <uirenderer/coordinate_transform.h>

MapRenderObjects __objs;
struct hashmap* __nodesmap;

MapRenderObjects* getObjs() {
  return &__objs;
}

struct hashmap* getNodes() {
  return __nodesmap;
}

bool initMap(FILE* f) {
  __objs = parseMapFileDescriptorToRenderObjects(f);
  RO_determineBoundsFromStreets(&__objs);
  __nodesmap = convertNodes(&__objs);

  return true;
}

void deinitMap() {
  destroyRenderObjects(&__objs);
  hashmap_free(__nodesmap);
}
