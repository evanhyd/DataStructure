//#define IN_USE
#ifdef IN_USE

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <malloc.h>
#include <stdio.h>

int main() {

  int* p = alloca(400 * sizeof(int));
  for (int i = 0; i < 400; ++i) {
    p[i] = i;
    printf("%d\n", p[i]);
  }
  return 0;
}

#endif

