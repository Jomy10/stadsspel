//
//  bufferReallocator.h
//  Stadsspel
//
//  Created by Jonas Everaert on 07/09/2023.
//

#ifndef bufferReallocator_h
#define bufferReallocator_h

#include <stdlib.h>

struct BufferReallocator {
    unsigned char* allocatedBuffer;
    size_t cap;
};

struct BufferReallocator br_init(void);

unsigned char* br_alloc(struct BufferReallocator*, size_t);
unsigned char* br_calloc(struct BufferReallocator*, size_t);

void br_deinit(struct BufferReallocator*);

#endif /* bufferReallocator_h */
