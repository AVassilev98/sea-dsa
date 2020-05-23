// clang-format off

// clang -O0 -c -emit-llvm -S -Xclang -disable-O0-optnone test-ext-3.c -o ../test-ext-3.ll

// clang-format on

#include "sea_dsa.h"
#include <stdlib.h>

/*Example of how to use sea_dsa_set_* attribute setters */

extern void print(int *x);
extern unsigned nd_uint(void);

int main(void) {

  {
    // Test multiple attributes set
    void *p = sea_dsa_new();
    sea_dsa_set_ptrtoint(p);
    sea_dsa_set_inttoptr(p);
    sea_dsa_set_modified(p);
    sea_dsa_set_read(p);
    sea_dsa_set_heap(p);
    sea_dsa_set_alloca(p);
  }

  {
    // Test sea_dsa_set_ptrtoint
    void *p = sea_dsa_new();
    sea_dsa_set_ptrtoint(p);
  }

  {
    // Test sea_dsa_set_ptrtoint
    void *p = sea_dsa_new();
    sea_dsa_set_inttoptr(p);
  }

  {
    // Test sea_dsa_set_modified
    void *p = sea_dsa_new();
    sea_dsa_set_modified(p);
  }

  {
    // Test sea_dsa_set_read
    void *p = sea_dsa_new();
    sea_dsa_set_read(p);
  }

  {
    // Test sea_dsa_set_heap
    void *p = sea_dsa_new();
    sea_dsa_set_heap(p);
  }

  {
    // Test sea_dsa_set_stack
    void *p = sea_dsa_new();
    sea_dsa_set_alloca(p);
  }
  return 0;
}