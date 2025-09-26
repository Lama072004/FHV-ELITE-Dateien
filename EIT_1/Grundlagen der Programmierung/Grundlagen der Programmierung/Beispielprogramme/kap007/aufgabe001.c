// kap007/aufgabe001.c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  float fval = multi(3.33);
  printf("%.2f\n", fval);
  return EXIT_SUCCESS;
}

float multi(float f) {
  return (f*f);
}
