#include <nav_view.h>

#include <stdlib.h>
#include <renderer/renderer.h>
#include <util/types.h>
#include <vec/vec.h>

#include <assert.h>

void renderNavView(Renderer gRendenderer, int renX, int renY, int renW, int renH, struct NavView* elements) {
  int navHeight = renH / 7;
  float navWidth = (float) renW / elements->elementsSize;

  assert(elements != null);
  assert(elements->elementsSize == 0 || elements->elements != null);

  for (int i = 0; i < elements->elementsSize; i++) {
    elements->elements[i].renderElement(
      gRendenderer,
      renX + navWidth * i,
      renY + renH - navHeight,
      (float) renW / elements->elementsSize,
      navHeight
    );
  }

  elements->elements[elements->currentView].renderContent(gRendenderer,
    renX, renY,
    renW, renH - navHeight
  );
}
