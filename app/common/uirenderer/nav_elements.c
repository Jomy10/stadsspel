#include <nav_elements.h>
#include <map_view.h>
#include <stdlib.h>
#include <util/types.h>
#include <map_data/render_objects.h>
#include <hashmap.h>

#include <stdio.h> // TODO: remove

void nav_element_renderMapElement(Renderer ren, int x, int y, int w, int h) {
  gDrawRect(ren, (GRect){
    x, y, w, h,
  }, &(GRectOpts){
    .fill = (GColor){100, 100, 0, 255 },
    .stroke = null,
  });
}

extern MapRenderObjects* __objs;
extern struct hashmap* __mapnodes;

void nav_element_renderMapContent(Renderer ren, int x, int y, int w, int h) {
  printf("rendering map\n");
  renderMapView(ren, x, y, w, h, __objs, __mapnodes);
}

void nav_element_renderScoreboardElement(Renderer ren, int x, int y, int w, int h) {
  gDrawRect(ren, (GRect){
    x, y, w, h,
  }, &(GRectOpts){
    .fill = (GColor){0, 100, 100, 255 },
    .stroke = null,
  });
}

void nav_element_renderScoreboardContent(Renderer ren, int x, int y, int w, int h) {
  gDrawRect(ren, (GRect){
    x, y, w, h,
  }, &(GRectOpts){
    .fill = (GColor){0, 100, 0, 255 },
    .stroke = null,
  });
}
