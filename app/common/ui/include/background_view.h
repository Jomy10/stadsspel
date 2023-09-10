//
//  background_view.h
//  Stadsspel
//
//  Created by Jonas Everaert on 09/09/2023.
//

#ifndef background_view_h
#define background_view_h

#include <ui/view.h>

typedef struct {
  arColor fillColor;
} BackgroundFillViewData;

arView* createBackgroundFillView(BackgroundFillViewData*, arView*);

#endif /* background_view_h */
