#ifndef _STADSSPEL_APP_H_
#define _STADSSPEL_APP_H_

#include <hashmap.h>
#include <stdbool.h>
#include <map_data/render_objects.h>
#include <map_data/parse.h>
#include <renderer/coordinate_transform.h>

bool init_app(MapRenderObjects* objs, struct hashmap** nodesmap) {
  *objs = parseMapToRenderObjects("map.o5m");
  RO_determineBoundsFromStreets(objs);
  *nodesmap = convertNodes(objs);

  return true;
}

void deinit_app(MapRenderObjects* objs, struct hashmap* nodesmap) {
  destroyRenderObjects(objs);
  hashmap_free(nodesmap);
}

#endif
