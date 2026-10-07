// kap008/listing004.c
#include <stdio.h>
#include <stdlib.h>
#define MINUS(a, b) (a-b)

int main(void) {
  printf("%f\n", MINUS(5.0, 2.5+0.5) );
  return EXIT_SUCCESS;
}
