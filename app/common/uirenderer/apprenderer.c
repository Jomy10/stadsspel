#include <apprenderer.h>
#include <map_view.h>
#include <nav_view.h>
#include <renderer/renderer.h>
#include <map_data/render_objects.h>
#include <nav_elements.h>

struct ViewState viewState = (struct ViewState) {
  .currentView = VIEW_MAIN,
  .mainView = {
    .navView = (struct NavView) {
      .currentView = 0,
      .elements = (struct NavViewElement[]){
        (struct NavViewElement){
          .renderElement = &nav_element_renderMapElement,
          .renderContent = &nav_element_renderMapContent,
        },
        (struct NavViewElement){
          .renderElement = &nav_element_renderScoreboardElement,
          .renderContent = &nav_element_renderScoreboardContent,
        }
      },
      .elementsSize = 2,
    },
  },
  ._navView = {
    .active = true,
  },
};

void renderMain(Renderer ren, GRect screenBounds);

void setView(enum View view) {
  switch (view) {
    case VIEW_MAIN:
      viewState._navView.active = true;
      viewState._navView.buttons.count = viewState.mainView.navView.elementsSize;
      viewState._navView.currentNavView = &viewState.mainView.navView;
  }
}

void renderApp(Renderer ren, MapRenderObjects* objs, struct hashmap* mapnodes, GRect screenBounds) {

  //renderMapView(ren, screenBounds.x, screenBounds.y, screenBounds.w, screenBounds.h, objs, mapnodes);
  //renderNavView(ren, screenBounds.x, screenBounds.y, screenBounds.w, screenBounds.h, &(struct NavView){ .elementsSize = 4 });

  switch (viewState.currentView) {
    case VIEW_MAIN:
      renderMain(ren, screenBounds);
      break;
  }

  gRenderUpdate(ren);
}

__attribute__((unused)) MapRenderObjects* __objs;
__attribute__((unused)) struct hashmap* __mapnodes;

void renderMain(Renderer ren, GRect screenBounds) {
  renderNavView(ren, screenBounds.x, screenBounds.y, screenBounds.w, screenBounds.h, &viewState.mainView.navView);
}
