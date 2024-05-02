// test.cc
#include <cstdio>
#include <stdlib.h>

int main() {
  for (int i = 1; i < 10; i++) {
    void *a = malloc(i);
    // printf("%p\n", a);
    // free(a);
  }
  return 0;
}

