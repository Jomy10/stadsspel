//
//  bufferReallocator.c
//  Stadsspel
//
//  Created by Jonas Everaert on 07/09/2023.
//

#include "bufferReallocator.h"

#include <string.h>

struct BufferReallocator br_init(void) {
    struct BufferReallocator br;
    br.allocatedBuffer = NULL;
    br.cap = 0;
    return br;
}

unsigned char* br_alloc(struct BufferReallocator* br, size_t cap) {
    if (br->cap == 0) {
        br->allocatedBuffer = malloc(cap);
        br->cap = cap;
    } else if (br->cap < cap) {
        br->allocatedBuffer = realloc(br->allocatedBuffer, cap);
        br->cap = cap;
    }
    
    return br->allocatedBuffer;
}

unsigned char* br_calloc(struct BufferReallocator* br, size_t cap) {
    br_alloc(br, cap);
    memset(br->allocatedBuffer, 0, cap);
    return br->allocatedBuffer;
}

void br_deinit(struct BufferReallocator* br) {
    free(br->allocatedBuffer);
}
