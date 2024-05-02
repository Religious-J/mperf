#include <stdio.h>
#include <stdlib.h>

int main() {
  // 使用malloc申请内存
  int *ptr1 = (int *)malloc(5 * sizeof(int));
  if (ptr1 == NULL) {
    printf("内存分配失败\n");
    return 1;
  }

  // 使用calloc申请内存，并初始化为零
  int *ptr2 = (int *)calloc(5, sizeof(int));
  if (ptr2 == NULL) {
    printf("内存分配失败\n");
    return 1;
  }

  // 使用realloc重新分配内存大小
  int *ptr3 = (int *)realloc(ptr2, 10 * sizeof(int));
  if (ptr3 == NULL) {
    printf("内存分配失败\n");
    return 1;
  }

  // 输出申请的内存空间的值
  printf("ptr1:\n");
  for (int i = 0; i < 5; i++) {
    printf("%d ", ptr1[i]);
  }
  printf("\n");

  printf("ptr3:\n");
  for (int i = 0; i < 10; i++) {
    printf("%d ", ptr3[i]);
  }
  printf("\n");

  // 释放内存
  free(ptr1);
  free(ptr3);

  return 0;
}
