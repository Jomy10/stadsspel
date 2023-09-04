#include <apprenderer.h>
#include <map_view.h>
#include <nav_view.h>
#include <renderer/renderer.h>
#include <map_data/render_objects.h>
#include <nav_elements.h>
#include <stdbool.h>

#include <stdio.h>

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
    .zoom = 1
  },
  .scale = 1
};

void setView(enum View view) {
  switch (view) {
    case VIEW_MAIN:
      viewState._navView.active = true;
      viewState._navView.buttons.count = viewState.mainView.navView.elementsSize;
      viewState._navView.currentNavView = &viewState.mainView.navView;
      break;
  }
}

void renderMain(Renderer ren, GRect screenBounds);
void renderApp(Renderer ren,/* MapRenderObjects* objs, struct hashmap* mapnodes,*/ GRect screenBounds) {
  printf("Rerendering app\n");

  gRenderClearColor(ren, (GColor){0,0,0,255});

  switch (viewState.currentView) {
    case VIEW_MAIN:
      renderMain(ren, screenBounds);
      break;
  }

  gRenderUpdate(ren);
}

void renderMain(Renderer ren, GRect screenBounds) {
  renderNavView(ren, screenBounds.x, screenBounds.y, screenBounds.w, screenBounds.h, &viewState.mainView.navView);
}
