#include "include/app.h"
#include "include/map.h"

#define OLIVEC_IMPLEMENTATION
#include <olive.c>

static struct AppState appState = (struct AppState){};

bool renderApp(arRect frame, bool forceRerender) {
  if (!forceRerender && !appState.shouldRerender) {
    if (recteql(&appState.previousRect, &frame)) {
      return false;
    }
  }

  printf("Rendering app\n");

  // Clear screen
  olivec_fill(appState.canvas, 0xFF000000);
  appState.rootView->render(appState.rootView, &appState.canvas, frame);

  appState.viewChanged = true;
  appState.shouldRerender = false;

  return true;
}

void handleNavViewTouch(arPoint pos) {
  int index = navview_touchedNavViewIndex(appState.navViewData, appState.previousRect, pos);

  if (index == -1) return;

  *appState.selectedNavItem = index;

  renderApp(appState.previousRect, true);
}

void initAppState(void) {
  appState.minMapCap = 1;
  appState.maxMapCap = 20;
  appState.previousRect = (arRect){-1,-1,-1,-1};
  appState.buffer = NULL;
  appState.reallocator = br_init();
  appState.uiAllocator = makeVariableArenaAllocator(1024);
  appState.mapnodes = NULL;
  ar_setAllocator(appState.uiAllocator);

  createRootView(
    &appState.rootView,
    &appState.mapViewLevel,
    &appState.mapScaleMid,
    &appState.navSize,
    &appState.selectedNavItem,
    &appState.navViewData,
    &appState.objs,
    (const struct hashmap**)&appState.mapnodes);

  appState.viewChanged = true;
}

void readMapData(FILE* f) {
  if (!initMap(f, &appState.objs, &appState.mapnodes)) {
    printf("Couldn't read map data\n");
  }
}

// (re-)init the bitmap buffer and canvas
void reinitAppWithFrame(arRect rect) {
  appState.buffer = br_alloc(&appState.reallocator, rect.w * rect.h * COLOR_BYTES_LEN);
  appState.canvas = olivec_canvas((uint32_t*)appState.buffer, rect.w, rect.h, rect.w);
  appState.previousRect = rect;
}

void destroyApp(void) {
  deinitMap(&appState.objs, appState.mapnodes);
  br_deinit(&appState.reallocator);
  vArenaAllocator_free(appState.uiAllocator);
}

unsigned char* getViewBitmap(void) {
  return appState.buffer;
}

arRect getPreviousFrame(void) {
  return appState.previousRect;
}

bool getDidViewChange(void) {
  if (appState.viewChanged) {
    appState.viewChanged = false;
    return true;
  }
  return false;
}

void capMapScale(float* toCap) {
  if (*toCap > appState.maxMapCap) *toCap = appState.maxMapCap;
  else if (*toCap < appState.minMapCap) *toCap = appState.minMapCap;
}

void scaleMap(float deltaScale) {
  *appState.mapViewLevel += deltaScale;
  capMapScale(appState.mapViewLevel);
}

void setMapScale(float scale) {
  *appState.mapViewLevel = scale;
  capMapScale(appState.mapViewLevel);
}

void setMapMid(arPoint point) {
  *appState.mapScaleMid = point;
}

void setShouldRerender(void) {
  appState.shouldRerender = true;
}

bool isViewMapView(void) {
  return appState.selectedNavItem == 0;
}
