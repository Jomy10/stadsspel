#ifndef _RENDERER_H
#define _RENDERER_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  void* ptr;
} Renderer;

typedef struct {
  float x;
  float y;
} GPoint;

typedef GPoint GSize;

typedef struct {
    uint8_t r, g, b, a;
} GColor;

typedef struct {
    float width;
    GColor color;
} GLineOpts;

void gDrawLine(Renderer, GPoint, GPoint);
void gDrawLineO(Renderer, GPoint, GPoint, GLineOpts*);

void gDrawLines(Renderer, const GPoint* points, int count);
void gDrawLinesO(Renderer, const GPoint* points, int count, GLineOpts*);

typedef struct {
    float x, y, w, h;
} GRect;

typedef struct {
    GColor fill;
    GLineOpts* stroke;
} GRectOpts;

void gDrawRect(Renderer, GRect);
void gDrawRectO(Renderer, GRect, GRectOpts*);

void gSetStrokeColor(Renderer, GColor);

void gSetFillColor(Renderer, GColor);

void gSetLineWidth(Renderer, float);

void gRenderUpdate(Renderer);

GSize gGetScreenSize(Renderer);

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