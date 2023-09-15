#ifndef _APP_RENDERER_H
#define _APP_RENDERER_H

#include <map_data/render_objects.h>
#include <hashmap.h>
#include <ui/view.h>
#include "bufferReallocator.h"
#include <arena/varena.h>
#include "root_ui.h"
#include <stdio.h>
#include <stdbool.h>

#undef OLIVEC_IMPLEMENTATION
#include <olive.c>

#define COLOR_BYTES_LEN 4

struct AppState {
  arView* rootView;
  MapRenderObjects objs;
  struct hashmap* mapnodes;
  struct BufferReallocator reallocator;
  unsigned char* buffer;
  Allocator* uiAllocator;
  arRect previousRect;
  Olivec_Canvas canvas;
  
  // UI State
  float* mapViewLevel;
  int* navSize;
  int* selectedNavItem;
  NavViewData* navViewData;
  
  /// Should the native renderer render the view?
  bool viewChanged;
};

/// Returns true if the UI has rerendered
bool renderApp(arRect frame, bool forceRerender);

void readMapData(FILE* f);

void handleNavViewTouch(arPoint pos);

void initAppState(void);

void reinitAppWithFrame(arRect rect);

void destroyApp(void);

unsigned char* getViewBitmap(void);

arRect getPreviousFrame(void);

bool getDidViewChange(void);

#endif
