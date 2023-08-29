#ifndef _MAP_VIEW_H
#define _MAP_VIEW_H

#include <map_data/render_objects.h>
#include <SDL3/SDL.h>
#include <hashmap.h>

void renderMapView(SDL_Renderer* gRenderer, int renX, int renY, int renW, int renH, MapRenderObjects* objs, struct hashmap* mapnodes);

#endif
