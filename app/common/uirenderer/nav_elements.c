#include <nav_elements.h>
#include <map_view.h>
#include <stdlib.h>
#include <util/types.h>
#include <map_data/render_objects.h>
#include <hashmap.h>
#include <app/app.h>
#include <apprenderer.h>
#include <renderer/renderer.h>
#include <list_view.h>
#include <app/scoreboard.h>
#include <list_item_view.h>

extern struct ViewState viewState;

void nav_element_renderMapElement(Renderer ren, int x, int y, int w, int h) {
  gDrawRect(ren, (GRect){
    x, y, w, h,
  }, &(GRectOpts){
    .fill = (GColor){100, 100, 0, 255 },
    .stroke = null,
  });
}

void nav_element_renderMapContent(Renderer ren, int x, int y, int w, int h) {
  renderMapView(ren, x, y, w, h, viewState._navView.zoom, getObjs(), getNodes());
}

void nav_element_renderScoreboardElement(Renderer ren, int x, int y, int w, int h) {
  gDrawRect(ren, (GRect){
    x, y, w, h,
  }, &(GRectOpts){
    .fill = (GColor){0, 100, 255, 255 },
    .stroke = null,
  });
}

void __renderScoreboardLeft(Renderer ren, int x, int y, int w, int h, int i) {
  // TODO: render number
  gDrawRect(ren, (GRect){x,y,w,h}, &(GRectOpts){
    .fill = (GColor){200,0,0,255},
    .stroke = null,
  });

  GSurface surface = gCreateSurface(ren, w, h);
  GFont font = gLoadFont("Roboto-Italic.ttf");
  gDrawText(&surface, font, "1", &(GFontOpts){
    .fill = (GColor){0,0,0,255},
    .lineHeight = 0.005,
  });
  gDrawSurface(ren, &surface, (GPoint){x, y});
  gFreeSurface(&surface);
  gFreeFont(font);
}

void __renderScoreboardRight(Renderer ren, int x, int y, int w, int h, char* txt) {
  // TODO: render text
  printf("Rendering right at (%i, %i, %i, %i)\n", x, y, w, h);
  gDrawRect(ren, (GRect){x,y,w,h}, &(GRectOpts){
    .fill = (GColor){255,0,0,255},
    .stroke = null,
  });
}

void nav_element_renderScoreboardContent(Renderer ren, int x, int y, int w, int h) {
  struct Scoreboard* scoreboard = getScoreboard();
  printf("Rendering scoreboard with %i entries\n", scoreboard->teamCount);
  struct ListViewItem items[scoreboard->teamCount];
  gDrawRect(ren, (GRect){x,y,w,h}, &(GRectOpts){
    .fill = (GColor){0, 100, 10, 255},
    .stroke = null,
  });
  for (int i = 0; i < scoreboard->teamCount; i++) {
    items[i] = (struct ListViewItem) {
      .renderLeft = &__renderScoreboardLeft,
      .renderRight = &__renderScoreboardRight,
      .txt = scoreboard->teams[i].name,
      .border = 5,
      .borderColor = (GColor){0, 0, 0, 255},
    };
  }
  renderList(ren, x, y, w, h, w - 2 * 10, h / ((scoreboard->teamCount + 1) % 10), 10, 10, items, scoreboard->teamCount);
}
