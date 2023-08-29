#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include <stdbool.h>

extern void test_VA_alloc(void);
extern void test_VA_clear(void);

void exit_if(bool b) {
 if (b) {
   CU_cleanup_registry();
   exit(CU_get_error());
 }
}

int main(void) {
  if (CU_initialize_registry() != CUE_SUCCESS) {
    return CU_get_error();
  }

  // ARENA //
  CU_pSuite arenaSuite = CU_add_suite("arena", NULL, NULL);
  exit_if(arenaSuite == NULL);
  exit_if(
    CU_add_test(arenaSuite, "Variable Arena allocate", test_VA_alloc) == NULL ||
    CU_add_test(arenaSuite, "Variable Arena clear", test_VA_clear) == NULL
  );

  CU_basic_run_tests();
  exit_if(true);

  return 0;
}
