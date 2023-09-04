#ifndef _LIST_ITEM_VIEW_H
#define _LIST_ITEM_VIEW_H

#include <renderer/renderer.h>

struct ListViewItem {
  void(*renderLeft)(Renderer, int, int, int, int, int);
  void(*renderRight)(Renderer, int, int, int, int, char*);
  char* txt;
  int border;
  GColor borderColor;
};

void renderListItem(Renderer, int, int, int, int, int, __attribute__((nonnull)) struct ListViewItem*);

#endif
