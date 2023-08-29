#include <renderer/map_view.h>
#include <SDL3/SDL.h>
#include <util/log.h>
#include <util/types.h>
#include <stdbool.h>
#include <map_data/render_objects.h>
#include <map_data/parse.h>
#include <assert.h>
#include <renderer/coordinate_transform.h>

#ifndef HashMap
#define HashMap(T) struct hashmap*
#endif

bool init(bool fullSize, SDL_Window** gWindow, SDL_Renderer** gRenderer, SDL_Rect* gScreenRect) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    err(0, "Failed to initializez SDL: %s\n", SDL_GetError());
    return false;
  }

  if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
    warn(0, "Linear texture filtering not enabled!");
  }

  if (fullSize) {
    SDL_DisplayMode displayMode;
    if (SDL_GetCurrentDisplayMode(0, &displayMode) == 0) {
      gScreenRect->w = displayMode.w;
      gScreenRect->h = displayMode.h;
      info(0, "Found screen size %d, %d\n", displayMode.w, displayMode.h);
    }
  }

  *gWindow = SDL_CreateWindow("Stadsspel", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gScreenRect->w, gScreenRect->h, SDL_WINDOW_SHOWN);
  if (*gWindow == null) {
    err(0, "Window could not be created: %s\n", SDL_GetError());
    return false;
  }

  *gRenderer = SDL_CreateRenderer(*gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (*gRenderer == null) {
    err(0, "Renderer could not be created: %s\n", SDL_GetError());
    return false;
  }

  return true;
}

void handleEvent(const SDL_Event* e, bool* quit) {
  switch (e->type) {
    case SDL_QUIT:
      *quit = true;
      return;
  }
}

int main() {
  SDL_Window* gWindow = NULL;
  SDL_Renderer* gRenderer = NULL;
  SDL_Rect gScreenRect = { 0, 0, 320, 240 };

  if (!init(false, &gWindow, &gRenderer, &gScreenRect)) {
    exit(-1);
  }
  assert(gWindow != null);
  assert(gWindow != null);

  MapRenderObjects objs = parseMapToRenderObjects("map.o5m");
  //RO_determineBounds(&objs);
  RO_determineBoundsFromStreets(&objs);
  //printObjs(&objs);
  HashMap(Node) nodes = convertNodes(&objs);

  bool quit = false;

  SDL_Event e;

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      handleEvent(&e, &quit);
      e = (SDL_Event){0};
    }

    int w, h;
    SDL_GetRendererOutputSize(gRenderer, &w, &h);

    renderMapView(gRenderer, 0, 0, w, h, &objs, nodes);

    SDL_RenderPresent(gRenderer);
  }

  EXIT:
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}
