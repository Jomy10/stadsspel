#include <allocator.h>

Allocator createAllocator(
  void* a,
  void*(*alloc)(void*, size_t),
  void*(*realloc)(void*, void*, size_t),
  void (*free)(void*, void*),
  void (*destroy)(void*)
) {
  Allocator al = malloc(sizeof(Allocator));
  *al = (struct _Allocator){
    .allocator = a,
    .alloc = alloc,
    .realloc = realloc,
    .free = free,
    .destroy_allocator = destroy,
  };
  return al;
}

void freeAllocator(Allocator al) {
  if (al->destroy_allocator != NULL) {
    al->destroy_allocator(al->allocator);
  }
  free(al);
}


// C allocator //

static inline void* calloc_malloc(__attribute__((unused)) void* _a, size_t size) {
  return malloc(size);
}

static inline void* calloc_realloc(__attribute__((unused)) void* _a, void* ptr, size_t size) {
  return realloc(ptr, size);
}

static inline void calloc_free(__attribute__((unused)) void* _a, void* ptr) {
  free(ptr);
}

static inline void calloc_destroy(__attribute__((unused)) void* _a) {}

Allocator create_Callocator() {
  Allocator al = malloc(sizeof(Allocator));
  *al = (struct _Allocator){
    .allocator = NULL,
    .alloc = calloc_malloc,
    .realloc = calloc_realloc,
    .free = calloc_free,
    .destroy_allocator = calloc_destroy,
  };
  return al;
}
