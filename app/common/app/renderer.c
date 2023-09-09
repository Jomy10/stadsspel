#include "include/renderer.h"
#include <ui/view.h>

#define OLIVEC_IMPLEMENTATION
#include <olive.c>

void renderApp(Olivec_Canvas canvas, arView* root, arRect bounds) {
  olivec_fill(canvas, 0xFF000000);
  root->render(root, &canvas, bounds);
}
