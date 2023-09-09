#ifndef _VARENA_H_
#define _VARENA_H_

#include <stdlib.h>

typedef struct {
  void** regions;
  /// The size in bytes of one region
  size_t regionSize;
  /// The amount of regions allocated
  size_t allocatedRegions;
  /// The current byte offset in the current region
  size_t currentOffset;
  /// The index into `regions` we are currently allocating at
  size_t currentRegion;
} VariableArena;

/// Allocates a variable-sized arena
/// # Parameters
/// - regionSize: the size in bytes of one region
VariableArena makeVariableArena(size_t regionSize);

void vArena_free(VariableArena* arena);

/// Trying to allocate a size bigger than the region size is considered undefined behaviour
void* vArena_alloc(VariableArena* arena, size_t size);

/// Invalidate all objects allocated by the arena, but keep its memory
void vArena_reset(VariableArena* arena);

// Allocator //

#include <util/allocator.h>

Allocator* makeVariableArenaAllocator(size_t regionSize);

void vArenaAllocator_free(Allocator*);

#endif
