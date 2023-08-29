#ifndef _COORDINATE_TRANSFORM_H
#define _COORDINATE_TRANSFORM_H

#include <util/types.h>
#include <hashmap.h>
#include <map_data/render_objects.h>

#ifndef HashMap
#define HashMap(T) struct hashmap*
#endif

/// x and y coordinates are between 0 and 1 (inclusive)
/// This is multiplied by the desired width to get the screen map position
typedef struct {
  u64 id;
  f32 x;
  f32 y;
} MapNode;

/// Returns a hashmap which should be freed
HashMap(Node) convertNodes(MapRenderObjects*);

f32 co_flip(f32, f32 max);

#endif
