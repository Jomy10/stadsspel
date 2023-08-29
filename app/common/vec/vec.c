#include <vec.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct Vec* createVecM(int cap, int type_size) {
    //void* mem = malloc(sizeof(struct Vec) + type_size * cap);
    struct Vec* v = malloc(sizeof(struct Vec));
    if (cap > 0) {
        v->values = malloc(type_size * cap);
    } else {
        v->values = NULL;
    }
    v->size = 0;
    v->cap = cap;
    v->type_size = type_size;
    return v;
}

void vec_free(struct Vec* vec) {
    free(vec->values);
    free(vec);
}

void vec_append(struct Vec* vec, void* value) {
    assert(vec->type_size != 0);

    if (vec->size == vec->cap) {
        vec->cap *= 2;
        vec->values = (char*) realloc(vec->values, vec->cap * vec->type_size);
    }
    memcpy(&((char*)vec->values)[(vec->size++) * vec->type_size], value, vec->type_size);
}

void vec_clear(struct Vec* vec) {
  vec->size = 0;
}
