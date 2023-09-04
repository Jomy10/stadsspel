#include <renderer/renderer.h>
#include <SDL3/SDL.h>
#include <util/types.h>

static inline void gSetForLineOpts(SDL_Renderer* ren, GLineOpts* opts) {
  if (opts != null) {
    SDL_SetRenderDrawColor(ren, opts->color.r, opts->color.g, opts->color.b, opts->color.a);
    // TODO: line width
  }
}

void gDrawLine(Renderer gren, GPoint p1, GPoint p2, GLineOpts* opts) {
  SDL_Renderer* ren = (SDL_Renderer*) gren.ptr;
  gSetForLineOpts(ren, opts);
  SDL_RenderDrawLineF(ren, p1.x, p1.y, p2.x, p2.y);
}

void gDrawLines(Renderer gren, const GPoint *points, int count, GLineOpts* opts) {
  SDL_Renderer* ren = (SDL_Renderer*) gren.ptr;
  gSetForLineOpts(ren, opts);

  SDL_RenderDrawLinesF(ren, (const SDL_FPoint*)points, count);
}

void gDrawRect(Renderer gren, GRect grect, GRectOpts* opts) {
  SDL_Renderer* ren = (SDL_Renderer*) gren.ptr;
  const SDL_FRect rect = {grect.x, grect.y, grect.w, grect.h};
  if (opts != null) {
    const GColor* col = &opts->fill;
    if (col->a != 0) {
      SDL_SetRenderDrawColor(ren, col->r, col->g, col->b, col->a);

      SDL_RenderFillRectF(ren, &rect);
    }

    if (opts->stroke != null) {
      const GColor* outlinecol = &opts->stroke->color;
      SDL_SetRenderDrawColor(ren, outlinecol->r, outlinecol->g, outlinecol->b, outlinecol->a);
      SDL_RenderDrawRectF(ren, &rect);
    }
  } else {
    SDL_RenderFillRectF(ren, &rect);
  }
}

// font rendering in common/truetype

GSurface gCreateSurface(Renderer ren, int w, int h) {
  return (GSurface) {
    .texture = SDL_CreateTexture(
      ren.ptr, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
      w, h),
    .bytesPerPixel = 4,
    .h = h,
    .w = w,
    .pixels = calloc(w * h, 4),
  };
}

void gFreeSurface(GSurface* surface) {
  free(surface->pixels);
  SDL_DestroyTexture(surface->texture);
}

void gDrawSurface(Renderer ren, GSurface* surface, GPoint pos) {
  SDL_UpdateTexture(
    surface->texture,
    0,
    surface->pixels,
    surface->w * surface->bytesPerPixel
  );
  SDL_RenderCopyF(ren.ptr, surface->texture, 0, &(SDL_FRect){
    pos.x, pos.y, surface->w, surface->h
  });
}

GISize gGetScreenSize(Renderer gren) {
  SDL_Renderer* ren = (SDL_Renderer*) gren.ptr;
  int w, h;
  SDL_GetRendererOutputSize(ren, &w, &h);
  return (GISize){w, h};
}

void gSetStrokeColor(Renderer gren, GColor c) {
  SDL_Renderer* ren = (SDL_Renderer*) gren.ptr;
  SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, c.a);
}

void gSetFillColor(Renderer gren, GColor c) {
  // TODO
}

void gSetLineWidth(Renderer gren, float w) {
  // TODO
}

void gRenderUpdate(Renderer gren) {
  SDL_RenderPresent(gren.ptr);
}

void gRenderClear(Renderer gren) {
  SDL_Renderer* ren = (SDL_Renderer*) gren.ptr;
  SDL_RenderClear(ren);
}

void gRenderClearColor(Renderer gren, GColor color) {
  SDL_Renderer* ren = (SDL_Renderer*) gren.ptr;

  int w, h;
  SDL_GetRendererOutputSize(ren, &w, &h);

  SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect(ren, &(SDL_Rect){0, 0, w, h});
}
