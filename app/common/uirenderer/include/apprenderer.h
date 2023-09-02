#ifndef _RENDERER_MAIN_H
#define _RENDERER_MAIN_H

#include <renderer/renderer.h>
#include "nav_view.h"
#include <map_data/render_objects.h>
#include <hashmap.h>

enum View {
  VIEW_MAIN,
};

struct ViewState {
  enum View currentView;
  struct {
    struct NavView navView;
  } mainView;
  struct {
    bool active;
    struct {
      int count;
    } buttons;
    struct NavView* currentNavView;
  } _navView;
  bool shouldRender;
};

void setView(enum View view);

void renderApp(Renderer ren, MapRenderObjects* objs, struct hashmap* mapnodes, GRect screenBound);

#endif
