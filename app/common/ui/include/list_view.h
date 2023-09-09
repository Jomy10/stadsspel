//
//  list_view.h
//  Stadsspel
//
//  Created by Jonas Everaert on 09/09/2023.
//

#ifndef list_view_h
#define list_view_h

#include <vec/veca.h>
#include <ui/view.h>
#include <util/tagged_union.h>

typedef struct {
  int subviewHeight;
  int itemMargin;
} UniformListViewData;

arView* createUniformListView(UniformListViewData* data);

#endif /* list_view_h */
