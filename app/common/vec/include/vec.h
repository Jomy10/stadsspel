#ifndef _VEC_H
#define _VEC_H

struct Vec {
    void* values;
    int size;
    int cap;
    int type_size;
};

#define Vec(T) struct Vec*

#define createVec(T, cap) createVecM(cap, sizeof(T))
struct Vec* createVecM(int cap, int type_size);

void vec_free(struct Vec* vec);

/// Copies the value to the vec
void vec_append(struct Vec*, void* value);

/// Invalidates all elements, but keeps the allocated memory
void vec_clear(struct Vec* vec);

#endif
