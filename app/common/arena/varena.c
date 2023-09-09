#include <varena.h>
#include <assert.h>

VariableArena makeVariableArena(size_t regionSize) {
  VariableArena arena = (VariableArena) {
    .regions = (void**) malloc(1 * sizeof(void*)),
    .regionSize = regionSize,
    .allocatedRegions = 1,
    .currentOffset = 0,
    .currentRegion = 0,
  };

  arena.regions[0] = malloc(regionSize);

  return arena;
}

void vArena_free(VariableArena* arena) {
  for (size_t i = 0; i < arena->allocatedRegions; ++i) {
    free(arena->regions[i]);
  }

  free(arena->regions);
}

void* vArena_alloc(VariableArena* arena, size_t size) {
  assert(arena->regionSize >= size);

  if (arena->currentOffset + size > arena->regionSize) {
    arena->currentRegion += 1;
    arena->currentOffset = 0;
    if (arena->allocatedRegions > arena->currentRegion) {
      // Don't allocate a new region

    } else {
      // Allocate a new region
      arena->allocatedRegions += 1;
      arena->regions = (void**) realloc(arena->regions, (arena->allocatedRegions) * sizeof(void**));
      arena->regions[arena->currentRegion] = malloc(arena->regionSize);
    }
  }

  void* ptr = (void*) (((char*) arena->regions[arena->currentRegion]) + arena->currentOffset);
  arena->currentOffset += size;

  return ptr;
}

void vArena_reset(VariableArena* arena) {
  arena->currentOffset = 0;
  arena->currentRegion = 0;
}

// Allocator //

#include <util/allocator.h>

static void* __vArena_realloc(void* allocator, __attribute__((unused)) void* ptr, size_t size) {
  return vArena_alloc(allocator, size);
}

static void __vArena_free(__attribute__((unused)) void* allocator, __attribute__((unused)) void* ptr) {}

// TODO
Allocator* makeVariableArenaAllocator(size_t regionSize) {
  VariableArena* arena = malloc(sizeof(VariableArena));
  *arena = makeVariableArena(regionSize);
  return createAllocator(
    arena,
    (void*(*)(void*, size_t)) &vArena_alloc,
    (void*(*)(void*, void*, size_t)) &__vArena_realloc,
    (void (*)(void*, void*)) &__vArena_free,
    (void (*)(void*)) vArena_free
  );
}

void vArenaAllocator_free(Allocator* allocator) {
  VariableArena* arena = allocator->allocator;
  free(arena);
  freeAllocator(allocator);
}
