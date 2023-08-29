#ifndef _VECA_H
#define _VECA_H

#include <util/allocator.h>

struct VecA {
    void* values;
    int size;
    int cap;
    int type_size;
    Allocator allocator;
};

#define VecA(T) struct VecA*

#define createVecA(T, cap, allocator) createVecAM((cap), sizeof(T), (allocator))
struct VecA* createVecAM(int cap, int type_size, Allocator a);

void veca_free(struct VecA* veca);

/// Copies the value to the veca
void veca_append(struct VecA*, void* value);

/// Invalidates all elements, but keeps the allocated memory
void veca_clear(struct VecA* veca);

#endif
