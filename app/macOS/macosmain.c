#include <hashmap.h>
#include <util/types.h>
#include <renderer/renderer.h>
#include <uirenderer/apprenderer.h>
#include <map_data/render_objects.h>
#include <map_data/parse.h>
#include <uirenderer/coordinate_transform.h>
#include <SDL3/SDL.h>
#include <app/app.h>
#include <time.h>

extern struct ViewState viewState;

bool init(SDL_Window** gWindow, SDL_Renderer** gRenderer) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_LogError(0, "Failed to initialize SDL: %s\n", SDL_GetError());
    return false;
  }

  if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
    SDL_LogWarn(0, "Linearr texture filtering not enabled!\n");
  }

  int w = 320, h = 240;
  *gWindow = SDL_CreateWindow("Stadsspel", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_METAL);
  if (*gWindow == null) {

    SDL_LogError(0, "Window could not be created: %s\n", SDL_GetError());
    return false;
  }

  *gRenderer = SDL_CreateRenderer(*gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (*gRenderer == null) {
    SDL_LogError(0, "Renderer could not be created: %s\n", SDL_GetError());
    return false;
  }
  int rw, rh;
  if (SDL_GetRendererOutputSize(*gRenderer, &rw, &rh) != 0) {
    SDL_LogWarn(0, "%s\n", SDL_GetError());
    return true;
  }
  viewState.scale = (float) rh / (float) h; // TODO: warn if scaleX != scaleY

  return true;
}

static SDL_Renderer* __ren;

int navClicked(const SDL_MouseButtonEvent* e) {
  int w = 0, h = 0;
  if (SDL_GetRendererOutputSize(__ren, &w, &h) != 0) {
    printf("%s\n", SDL_GetError());
  }
  printf("renderer: %i %i - %i %i\n", w, h, e->x, e->y);
  float navHeight = (float) h / 7;
  float navWidth = (float) w / viewState._navView.buttons.count;
  float navY = h - navHeight;
  int mouseY = e->y * viewState.scale;
  int mouseX = e->x * viewState.scale;
  printf("mouse: %d %d\n", mouseX, mouseY);

  if (mouseY >= navY && mouseY <= h) {
    // Clicked inside of navigation buttons
    for (int i = 0; i < viewState._navView.buttons.count; i++) {
      printf("nav: %f  %f\n", navWidth, navHeight);
      if (mouseX >= i * navWidth && mouseX <= navWidth + i * navWidth) {
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
          if (selectedNavElement == -1) break;
          viewState._navView.currentNavView->currentView = selectedNavElement;
          *shouldRender = true;
        }
      }
      return;
    case SDL_WINDOWEVENT_RESIZED:
      *shouldRender = true;
      return;
    case SDL_KEYDOWN:
      switch (e->key.keysym.scancode) {
        case SDLK_PLUS:
          viewState._navView.zoom += 0.1;
          break;
        case SDLK_MINUS:
          viewState._navView.zoom -= 0.1;
          break;
        default: break;
      }
      return;
  }
}

int main() {
  FILE* f = fopen("map.o5m", "rb");
  initMap(f);
  fclose(f);

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
      renderApp(renderer, appBounds);
      viewState.shouldRender = false;
    }
    struct timespec ts = {0, 1000};
    nanosleep(&ts, &ts);
  }

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  deinitMap();
  SDL_Quit();
}
