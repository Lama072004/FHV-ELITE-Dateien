// kap007/loesung001.c
#include <stdio.h>
#include <stdlib.h>

float multi(float);

int main(void) {
  float fval = multi(3.33);
  printf("%.2f\n", fval);
  return EXIT_SUCCESS;
}

float multi(float f) {
  return (f*f);
}
