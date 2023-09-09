#ifndef _ALLOCATOR_H
#define _ALLOCATOR_H

#include <stdlib.h>

struct _Allocator {
    void* allocator;
    void* (*alloc)(void*, size_t);
    void* (*realloc)(void*, void* ptr, size_t);
    void (*free)(void*, void* ptr);
    void (*destroy_allocator)(void*);
};

typedef struct _Allocator Allocator;

Allocator* createAllocator(
  void*,
  void*(*)(void*, size_t),
  void*(*)(void*, void*, size_t),
  void (*)(void*, void*),
  void (*)(void*)
);

#define al_alloc(_allocator, bytes) (_allocator)->alloc((_allocator)->allocator, bytes)
#define al_realloc(_allocator, ptr, bytes) (_allocator)->realloc((_allocator)->allocator, ptr, bytes)
#define al_free(_allocator, ptr) (_allocator)->free((_allocator)->allocator, ptr)

/// frees the whole allocator
void freeAllocator(Allocator*);

/// Default C allocator
Allocator* create_callocator();

#endif
