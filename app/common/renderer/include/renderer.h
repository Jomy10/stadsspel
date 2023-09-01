#ifndef _RENDERER_MAIN_H
#define _RENDERER_MAIN_H

#include <render_frontend/renderer.h>
#include <map_data/render_objects.h>
#include <hashmap.h>

void renderApp(Renderer ren, MapRenderObjects* objs, struct hashmap* mapnodes, GRect screenBound);

#endif
