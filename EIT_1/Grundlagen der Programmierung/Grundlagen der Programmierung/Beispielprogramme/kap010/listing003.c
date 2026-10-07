// kap010/listing003.c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  float *fptr;
  float fval1 = 123.123f, fval2;
  // fptr die Adresse von fval1 übergeben
  fptr =& fval1;
  // fval2 erhält den Wert von fval1
  fval2 = *fptr;
  printf("fval2: %.3f\n", fval2);
  return EXIT_SUCCESS;
}
