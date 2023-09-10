//
//  scoreboard_nav_item.h
//  Stadsspel
//
//  Created by Jonas Everaert on 09/09/2023.
//

#ifndef scoreboard_nav_item_h
#define scoreboard_nav_item_h

#include "view.h"

typedef struct {
    int rang;
    char* name;
} ScoreboardViewData;

arView* createScoreboardViewContent(ScoreboardViewData* data);
arView* createScoreboardViewNavItem(void);

#endif /* scoreboard_nav_item_h */
