// #include "o5mreader.h"
#include <render_objects.h>
#include <stdio.h>
#include <o5mreader.h>
#include <parse.h>

/// The returned value should be freed
MapRenderObjects parseMapToRenderObjects(const char* filename) {
  FILE* f = fopen(filename, "rb");
  MapRenderObjects objs = parseMapFileDescriptorToRenderObjects(f);
  fclose(f);
  return objs;
}

/// The returned value should be freed
MapRenderObjects parseMapFileDescriptorToRenderObjects(FILE* f) {
  MapRenderObjects objs = makeRenderObjects();

  O5mreader* reader;
  O5mreaderDataset ds;
  O5mreaderIterateRet ret, ret2;
  char *key, *val;
  u64 nodeId;

  o5mreader_open(&reader, f);

  void* way = initWay();

  while ((ret = o5mreader_iterateDataSet(reader, &ds)) == O5MREADER_ITERATE_RET_NEXT) {
    switch (ds.type) {
      case O5MREADER_DS_NODE:
        addNode(&objs, (Node) {
          .id = ds.id,
          .lat = ds.lat,
          .lon = ds.lon,
        });
        break;
      case O5MREADER_DS_WAY:
        setWay(way, ds.id);

        while ((ret2 = o5mreader_iterateNds(reader, &nodeId)) == O5MREADER_ITERATE_RET_NEXT) {
          way_addNode(way, nodeId);
        }

        while ((ret2 = o5mreader_iterateTags(reader, &key, &val)) == O5MREADER_ITERATE_RET_NEXT) {
          way_addTag(way, key, val);
        }

        wayToRenderObject(&objs, way);
    }
  }

  o5mreader_close(reader);
  freeWay(way);

  return objs;
}
