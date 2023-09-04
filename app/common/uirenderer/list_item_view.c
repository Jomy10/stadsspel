#include <list_item_view.h>
#include <renderer/renderer.h>
#include <assert.h>
#include <util/types.h>
#include <stdlib.h>

void renderListItem(Renderer ren, int renX, int renY, int renW, int renH, int i, __attribute__((nonnull)) struct ListViewItem* item) {
  int iconWidth = renW / 7;

  assert(item != null);
  assert(item->renderLeft != null);
  assert(item->renderRight != null);

  item->renderLeft(ren, renX, renY, iconWidth, renH, i);
  item->renderRight(ren, renX + iconWidth, renY, renW - iconWidth, renH, item->txt);
  // Outline
  if (item->border > 0) {
    gDrawRect(ren, (GRect){
      renX, renY, renW, renH
    }, &(GRectOpts){
      .fill = (GColor){0,0,0,0},
      .stroke = &(GLineOpts){
        .color = item->borderColor,
        .width = item->border,
      },
    });
  }
}
