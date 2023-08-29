#include <arena/varena.h>
#include <CUnit/CUnit.h>
#include <stdbool.h>
#include <stdio.h>

struct MyStruct {
  int a;
  int b;
  short c;
  bool d;
  long e;
};

void test_VA_alloc(void) {
  VariableArena va = makeVariableArena(sizeof(struct MyStruct));
  // Allocate two regions
  printf("%p %lu %lu %lu %lu\n", va.regions, sizeof(struct MyStruct), va.currentRegion, va.currentOffset, va.allocatedRegions);
  struct MyStruct* s = (struct MyStruct*) vArena_alloc(&va, sizeof(struct MyStruct));
  s->a = 1;
  s->b = 2;
  s->c = 3;
  s->d = false;
  s->e = 4;
  printf("%p %lu %lu %lu %lu\n", va.regions, sizeof(struct MyStruct), va.currentRegion, va.currentOffset, va.allocatedRegions);
  struct MyStruct* s2 = (struct MyStruct*) vArena_alloc(&va, sizeof(struct MyStruct));

  CU_ASSERT_PTR_NOT_EQUAL(s, s2);

  CU_ASSERT_EQUAL(s->a, 1);
  CU_ASSERT_EQUAL(s->b, 2);
  CU_ASSERT_EQUAL(s->c, 3);
  CU_ASSERT_EQUAL(s->d, false);
  CU_ASSERT_EQUAL(s->e, 4);

  s2->e = 6;
  CU_ASSERT_EQUAL(s2->e, 6);

  printf("%p %lu %lu %lu %lu\n", va.regions, sizeof(struct MyStruct), va.currentRegion, va.currentOffset, va.allocatedRegions);
  CU_ASSERT_EQUAL(va.allocatedRegions, 2);
  vArena_free(&va);
}

void test_VA_clear(void) {
  VariableArena va = makeVariableArena(sizeof(struct MyStruct));
  // Allocate two regions
  struct MyStruct* s = (struct MyStruct*) vArena_alloc(&va, sizeof(struct MyStruct));
  s->a = 1;
  s->b = 2;
  s->c = 3;
  s->d = false;
  s->e = 4;

  struct MyStruct* s2 = (struct MyStruct*) vArena_alloc(&va, sizeof(struct MyStruct));

  // Clear and reallocate
  vArena_reset(&va);

  struct MyStruct* s3 = (struct MyStruct*) vArena_alloc(&va, sizeof(struct MyStruct));
  CU_ASSERT_EQUAL(va.allocatedRegions, 2);
  struct MyStruct* s4 = (struct MyStruct*) vArena_alloc(&va, sizeof(struct MyStruct));
  CU_ASSERT_EQUAL(va.allocatedRegions, 2);
  struct MyStruct* s5 = (struct MyStruct*) vArena_alloc(&va, sizeof(struct MyStruct));
  CU_ASSERT_EQUAL(va.allocatedRegions, 3);

  CU_ASSERT_PTR_EQUAL(s, s3);
  CU_ASSERT_PTR_EQUAL(s2, s4);

  vArena_free(&va);
}
