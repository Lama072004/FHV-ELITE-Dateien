// kap008/listing009.c
#include <stdio.h>
#include <stdlib.h>
#include "modulA.h"

int main(void) {
  double dval = kreisflaeche(5.5);
  printf("Kreisflaeche        : %lf\n", dval);
  printf("Wert von modulA_var : %d\n", modulA_var);
  return EXIT_SUCCESS;
}
