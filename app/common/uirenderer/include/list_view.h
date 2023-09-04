#ifndef _LIST_VIEW_H
#define _LIST_VIEW_H

#include <renderer/renderer.h>
#include <list_item_view.h>

void renderList(Renderer, int, int, int, int, int, int, int itemMargin, int horMargin, struct ListViewItem* items, int);

#endif
