#include <veca.h>
#include <assert.h>
#include <string.h>
#include <util/allocator.h>

struct VecA* createVecAM(int cap, int type_size, Allocator a) {
  //void* mem = a->alloc(a->allocator, sizeof(struct VecA) + type_size * cap);
  struct VecA* v = a->alloc(a->allocator, sizeof(struct VecA));
  if (cap > 0) {
    v->values = a->alloc(a->allocator, type_size * cap); //mem + sizeof(struct VecA);
  } else {
    v->values = NULL;
  }

  v->size = 0;
  v->cap = cap;
  v->type_size = type_size;
  return v;
}

void veca_free(struct VecA* vec) {
  vec->allocator->free(vec->allocator->allocator, vec->values);
  vec->allocator->free(vec->allocator->allocator, vec);
}

void veca_append(struct VecA* vec, void* value) {
  assert(vec->type_size != 0);

  if (vec->size == vec->cap) {
    vec->cap *= 2;
    vec->values = (char*) vec->allocator->realloc(vec->allocator->allocator, vec->values, vec->cap * vec->type_size);
  }
  memcpy(&((char*)vec->values)[(vec->size++) * vec->type_size], value, vec->type_size);
}

void veca_clear(struct VecA* vec) {
  vec->size = 0;
}
