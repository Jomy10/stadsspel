#include "include/coordinate_transform.h"

#include <hashmap.h>
#include <map_data/render_objects.h>
#include <util/types.h>
#include <mercator.h>
#include <math.h>

// i32 coordinate to float
#define CO_PREC 10000000

#ifndef HashMap
#define HashMap(T) struct hashmap*
#endif

u64 mnode_hash(const void* item, __attribute__((unused)) u64 seed0, __attribute__((unused)) u64 seed1) {
  return ((const MapNode*)item)->id;
}

int mnode_cmp(const void* a, const void* b, __attribute__((unused)) void* udata) {
  return ((const MapNode*)a)->id - ((const MapNode*)b)->id;
}

HashMap(Node) convertNodes(MapRenderObjects* objs) {
  HashMap(Node) nodes = hashmap_new(sizeof(MapNode), 1000, 0, 0, &mnode_hash, &mnode_cmp, NULL, NULL);

  Bounds* bounds = RO_getBounds(objs);
  double minlat_d = lat2y_d((double)bounds->minlat / CO_PREC);
  double maxlat_d = lat2y_d((double)bounds->maxlat / CO_PREC);
  double minlon_d = lon2x_d((double)bounds->minlon / CO_PREC);
  double maxlon_d = lon2x_d((double)bounds->maxlon / CO_PREC);

  struct NodesIterator nodesIter = RO_nodesIter(objs);
  Node* entry = RO_nodesIter_next(&nodesIter);
  while (entry != null) {
    hashmap_set(nodes, &((MapNode){
      .id = entry->id,
      .x = (lon2x_d(((double)entry->lon) / CO_PREC) - minlon_d) / (maxlon_d - minlon_d),
      // flip to have north face up
      .y = co_flip((lat2y_d(((double)entry->lat) / CO_PREC) - minlat_d) / (maxlat_d - minlat_d), 1),
    }));

    entry = RO_nodesIter_next(&nodesIter);
  }
  RO_destroyNodesIter(&nodesIter);

  return nodes;
}


f32 co_flip(f32 p, f32 max) {
  return max - p;
}
