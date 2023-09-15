#include <hashmap.h>
#include <stdbool.h>
#include <map_data/render_objects.h>
#include <map_data/parse.h>
//#include <uirenderer/coordinate_transform.h>
#include <ui/coordinate_transform.h>

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
