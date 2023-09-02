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
    SDL_SetRenderDrawColor(ren, col->r, col->g, col->b, col->a);

    SDL_RenderFillRectF(ren, &rect);

    if (opts->stroke != null) {
      const GColor* outlinecol = &opts->stroke->color;
      SDL_SetRenderDrawColor(ren, outlinecol->r, outlinecol->g, outlinecol->b, outlinecol->a);
      SDL_RenderDrawRectF(ren, &rect);
    }
  } else {
    SDL_RenderFillRectF(ren, &rect);
  }
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
