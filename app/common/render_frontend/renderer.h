#ifndef _RENDERER_H
#define _RENDERER_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
  void* ptr;
} Renderer;

typedef struct {
  float x;
  float y;
} GPoint;

typedef GPoint GSize;

typedef struct {
  int x;
  int y;
} GIPoint;

typedef GIPoint GISize;

typedef struct {
    uint8_t r, g, b, a;
} GColor;

typedef struct {
    float width;
    GColor color;
} GLineOpts;

//void gDrawLine(Renderer, GPoint, GPoint);
void gDrawLine(Renderer, GPoint, GPoint, GLineOpts*);

// void gDrawLines(Renderer, const GPoint* points, int count);
void gDrawLines(Renderer, const GPoint* points, int count, GLineOpts*);

typedef struct {
    float x, y, w, h;
} GRect;

typedef struct {
    GColor fill;
    GLineOpts* stroke;
} GRectOpts;

//void gDrawRect(Renderer, GRect);
void gDrawRect(Renderer, GRect, GRectOpts*);

typedef void* GFont;

GFont gLoadFont(char* fontFile);
GFont gLoadFontFileDescriptor(FILE*);
void gFreeFont(GFont);

typedef struct {
  GColor fill;
  float lineHeight;
} GFontOpts;

typedef struct {
  void* texture; // e.g. SDL_Texture
  int bytesPerPixel;
  int w;
  int h;
  unsigned char* pixels;
} GSurface;

/// Renders a font into a surface
void gDrawText(GSurface*, GFont, const char* text, const GFontOpts*);

GSurface gCreateSurface(Renderer, int w, int h);
void gFreeSurface(GSurface*);

void gDrawSurface(Renderer, GSurface*, GPoint);

/// deprecated
void gSetStrokeColor(Renderer, GColor);

/// deprecated
void gSetFillColor(Renderer, GColor);

/// deprecated
void gSetLineWidth(Renderer, float);


void gRenderUpdate(Renderer);

void gRenderClear(Renderer);
/// clear the screen with the specified color
void gRenderClearColor(Renderer, GColor);

GISize gGetScreenSize(Renderer);

//typedef enum {
//    G_ET_Click,
//} GEventType;
//
//typedef union {
//    /// **Click**
//    GPoint pos;
//} GEventData;
//
//typedef struct {
//    GEventType type;
//    GEventData data;
//} GEvent;
//
//bool gPopEvent(GEvent*);

#endif
