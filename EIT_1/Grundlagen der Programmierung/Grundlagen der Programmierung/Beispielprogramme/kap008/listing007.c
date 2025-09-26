// kap008/listing007.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// #define NDEBUG
#include <assert.h>

int main(void) {
  double x = 0.0;
  printf("Bitte eine positive Gleitkommazahl: ");
  if( scanf("%lf", &x) != 1 ) {
    printf("Fehler bei der Eingabe...\n");
    return EXIT_FAILURE;
  }
  assert(x >= 0.0);
  double dval = sqrt(x);
  printf("%lf\n", dval);
  return EXIT_SUCCESS;
}
