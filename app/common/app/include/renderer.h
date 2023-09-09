#ifndef _APP_RENDERER_H
#define _APP_RENDERER_H

#include <map_data/render_objects.h>
#include <hashmap.h>
#include <ui/view.h>

#undef OLIVEC_IMPLEMENTATION
#include <olive.c>

void renderApp(Olivec_Canvas, arView*, arRect);

#endif
