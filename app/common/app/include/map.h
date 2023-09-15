#ifndef _STADSSPEL_APP_H_
#define _STADSSPEL_APP_H_

#include <hashmap.h>
#include <stdbool.h>
#include <map_data/render_objects.h>
#include <map_data/parse.h>

bool initMap(FILE*, MapRenderObjects*, struct hashmap**);
void deinitMap(MapRenderObjects*, struct hashmap*);

#endif
