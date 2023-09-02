#include <hashmap.h>
#include <util/types.h>
#include <renderer/renderer.h>
#include <uirenderer/apprenderer.h>
#include <map_data/render_objects.h>
#include <map_data/parse.h>
#include <uirenderer/coordinate_transform.h>
#include <SDL3/SDL.h>

bool init(SDL_Window** gWindow, SDL_Renderer** gRenderer) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_LogError(0, "Failed to initialize SDL: %s\n", SDL_GetError());
    return false;
  }

  if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
    SDL_LogWarn(0, "Linearr texture filtering not enabled!\n");
  }

  *gWindow = SDL_CreateWindow("Stadsspel", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 320, 240, SDL_WINDOW_SHOWN);
  if (*gWindow == null) {
    SDL_LogError(0, "Window could not be created: %s\n", SDL_GetError());
    return false;
  }

  *gRenderer = SDL_CreateRenderer(*gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (*gRenderer == null) {
    SDL_LogError(0, "Renderer could not be created: %s\n", SDL_GetError());
    return false;
  }

  return true;
}

extern struct ViewState viewState;

extern MapRenderObjects* __objs;
extern struct hashmap* __mapnodes;
static SDL_Renderer* __ren;

int navClicked(const SDL_MouseButtonEvent* e) {
  int w = 0, h = 0;
  if (SDL_GetRendererOutputSize(__ren, &w, &h) != 0) {
    printf("%s\n", SDL_GetError());
  }
  int navHeight = h / 7;
  float navWidth = (float) w / viewState._navView.buttons.count;
  float navY = h - navHeight;

  printf("screen = (%i, %i), nav = (%f, max:%i)\n", w, h, navY, h);
  if (e->y >= navY && e->y <= h) {
    printf("Clicked inside of nav\n");
    // Clicked inside of navigation buttons
    for (int i = 0; i < viewState._navView.buttons.count; i++) {
      if (e->x >= i * navWidth && e->x <= navWidth + i * navWidth) {
        return i;
      }
    }
  }

  return -1;
}

void handleEvent(const SDL_Event* e, bool* quit, bool* shouldRender) {
  switch (e->type) {
    case SDL_QUIT:
      *quit = true;
      return;
    case SDL_MOUSEBUTTONDOWN:
      if (e->button.button == SDL_BUTTON_LEFT) {
        if (viewState._navView.active) {
          int selectedNavElement = navClicked(&e->button);
          printf("%i\n", selectedNavElement);
          if (selectedNavElement == -1) break;
          viewState._navView.currentNavView->currentView = selectedNavElement;
          *shouldRender = true;
        }
      }
      return;
    case SDL_WINDOWEVENT_RESIZED:
      viewState.shouldRender = true;
      return;
  }
}

int main() {
  FILE* f = fopen("map.o5m", "rb");

  MapRenderObjects objs = parseMapFileDescriptorToRenderObjects(f);
  fclose(f);
  RO_determineBoundsFromStreets(&objs);
  printObjsSummary(&objs);
  struct hashmap* mapnodes = convertNodes(&objs);
  __objs = &objs;
  __mapnodes = mapnodes;

  SDL_Window* win = NULL;
  SDL_Renderer* ren = NULL;

  if (!init(&win, &ren)) {
    return -1;
  }

  __ren = ren;

  Renderer renderer = (Renderer){ren};
  setView(VIEW_MAIN);

  bool quit = false;
  SDL_Event e;
  viewState.shouldRender = true;

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      handleEvent(&e, &quit, &viewState.shouldRender);
    }

    if (viewState.shouldRender) {
      GISize s = gGetScreenSize(renderer);
      GRect appBounds = (GRect){0, 0, s.x, s.y};
      renderApp(renderer, &objs, mapnodes, appBounds);
      viewState.shouldRender = false;
    }
  }

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  destroyRenderObjects(&objs);
  hashmap_free(mapnodes);
  SDL_Quit();
}
