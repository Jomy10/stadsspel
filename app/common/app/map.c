#include <hashmap.h>
#include <stdbool.h>
#include <map_data/render_objects.h>
#include <map_data/parse.h>
//#include <uirenderer/coordinate_transform.h>
#include <ui/coordinate_transform.h>

MapRenderObjects __objs;
struct hashmap* __nodesmap;

MapRenderObjects* getObjs() {
  return &__objs;
}

struct hashmap* getNodes() {
  return __nodesmap;
}

bool initMap(FILE* f, MapRenderObjects* objs, struct hashmap** nodesmap) {
  *objs = parseMapFileDescriptorToRenderObjects(f);
  RO_determineBoundsFromStreets(objs);
  *nodesmap = convertNodes(objs);

  return true;
}

void deinitMap(MapRenderObjects* objs, struct hashmap* nodesmap) {
  destroyRenderObjects(objs);
  hashmap_free(nodesmap);
}
