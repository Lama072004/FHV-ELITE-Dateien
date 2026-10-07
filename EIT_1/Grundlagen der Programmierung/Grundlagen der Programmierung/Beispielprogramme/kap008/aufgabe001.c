// kap008/aufgabe001.c
#include <stdio.h>
#include <stdlib.h>
#define MULTI(a, b) (a*b)

int main(void) {
  int val1 = 10, val2 = 20;
  printf("Multiplikation = %d\n", MULTI(val1, val2-10));
  return EXIT_SUCCESS;
}
