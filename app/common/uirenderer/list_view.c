#include <renderer/renderer.h>
#include <list_item_view.h>
#include <assert.h>
#include <util/types.h>
#include <stdlib.h>

#include <stdio.h> // TODO remove

void renderList(Renderer ren, int renX, int renY, int renW, int renH, int itemW, int itemH, int itemMargin, int horMargin, struct ListViewItem* items, int itemCount) {
  assert(itemCount == 0 || items != null);

  printf("Item count = %i\n", itemCount);
  for (int i = 0; i < itemCount; i++) {
    printf("item at %i %i with size %i %i\n", renX, renY + i * (itemH + itemMargin), itemW, itemH);
    renderListItem(ren, renX + horMargin, renY + i * (itemH + itemMargin) + itemMargin, itemW, itemH, i, &items[i]);
  }
}
