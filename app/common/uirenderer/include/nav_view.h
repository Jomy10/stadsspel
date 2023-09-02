#ifndef _NAV_VIEW_H
#define _NAV_VIEW_H

#include <vec/vec.h>
#include <renderer/renderer.h>

struct NavView {
  const struct NavViewElement* elements;
  const int elementsSize;
  int currentView;
};

struct NavViewElement {
  void(*renderElement)(Renderer, int, int, int, int);
  void(*renderContent)(Renderer, int, int, int, int);
};

void renderNavView(Renderer gRendenderer, int renX, int renY, int renW, int renH, struct NavView* elements);

#endif
