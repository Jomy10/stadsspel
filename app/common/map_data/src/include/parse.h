#ifndef _PARSE_H
#define _PARSE_H

#include "render_objects.h"
#include <stdio.h>

MapRenderObjects parseMapToRenderObjects(const char* filename);

MapRenderObjects parseMapFileDescriptorToRenderObjects(FILE* file);

#endif
