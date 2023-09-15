//
// ArgonUI
//

#ifndef _VIEW_H
#define _VIEW_H

#include <vec/veca.h>
#include <util/allocator.h>
#include <stdint.h>

#undef OLIVEC_IMPLEMENTATION
#include <olive.c>

Allocator* arAllocator;

typedef struct {
  int x, y, w, h;
} arRect;

typedef struct {
  int x, y;
} arPoint;

typedef arPoint arSize;

typedef uint32_t arColor;

bool recteql(arRect* rect1, arRect* rect2);

// Base view
typedef struct _arView {
  VecA(arView*) subViews;
  void* data;
  void(*render)(struct _arView* self, Olivec_Canvas* canvas, arRect bounds);
} arView;

arView* createArView(void);

void freeArView(arView*);

void ar_addSubView(arView*, arView*);

void ar_setAllocator(Allocator*);

#endif
