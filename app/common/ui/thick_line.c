#include "include/thick_line.h"
#include <math.h>

#define OLIVEC_IMPLEMENTATION
#include <olive.c>

void thicc_line(Olivec_Canvas canvas, int x1, int y1, int x2, int y2, int thickness, uint32_t color) {
  int dx = x2 - x1;
  int dy = y2 - y1;

  float length = sqrt(dx * dx + dy * dy);

  if ((length > 0) && (thickness > 0)) {
    float scale = thickness / (2 * length);

    float radiusx = -scale * dy;
    float radiusy =  scale * dx;

    float stripx[4] = {
      x1 - radiusx,
      x1 + radiusx,
      x2 - radiusx,
      x2 + radiusx,
    };
    float stripy[4] = {
      y1 - radiusy,
      y1 + radiusy,
      y2 - radiusy,
      y2 + radiusy,
    };

    for (int i = 2; i < 4; i++) {
      if ((i%2) == 0) {
        olivec_triangle(
          canvas,
          stripx[i], stripy[i],
          stripx[i - 2], stripy[i - 2],
          stripx[i - 1], stripy[i - 1],
          color);
      } else {
        olivec_triangle(
          canvas,
          stripx[i], stripy[i],
          stripx[i - 1], stripy[i - 1],
          stripx[i - 2], stripy[i - 2],
          color);
      }
    }
  }
}
