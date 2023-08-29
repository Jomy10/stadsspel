#ifndef _RENDER_OBJECTS_H
#define _RENDER_OBJECTS_H

#include <stdlib.h>
// #include <stdint.h>
// #define u64 uint64_t
// #define i32 int32_t
#include <util/types.h>

typedef struct CMapRenderObjects {
  void* ptr;
} MapRenderObjects;

MapRenderObjects makeRenderObjects();

void destroyRenderObjects(MapRenderObjects*);

void printObjs(MapRenderObjects*);

//== Constructing ==//
typedef struct CNode {
  u64 id;
  i32 lat;
  i32 lon;
} Node;

void addNode(MapRenderObjects*, Node);

void* initWay();
void freeWay(void*);
/// Resets all memory associated to a way and makes a new one
void setWay(void*, u64 id);
void way_addNode(void* way, u64 nodeId);
void way_addTag(void* way, const char* key, const char* value);

void wayToRenderObject(MapRenderObjects*, void* way);

typedef struct CRO_Street {
  char* type;
  u64* nodes;
  size_t nodes_count;
} RO_Street;

typedef struct CBounds {
  i32 minlat;
  i32 maxlat;
  i32 minlon;
  i32 maxlon;
} Bounds;

void RO_determineBounds(MapRenderObjects*);

void RO_determineBoundsFromStreets(MapRenderObjects*);

//== Accessing ==//

void RO_getStreets(MapRenderObjects*, RO_Street**, int* arr_size);

/// Returns NULL if the node could not be found
Node* RO_getNode(MapRenderObjects*, u64);

Bounds* RO_getBounds(MapRenderObjects*);

//=== Iterator ===//

struct NodesIterator {
  void* iterator;
};

struct NodesIterator RO_nodesIter(MapRenderObjects*);
void RO_destroyNodesIter(struct NodesIterator*);

Node* RO_nodesIter_next(struct NodesIterator*);

#endif
