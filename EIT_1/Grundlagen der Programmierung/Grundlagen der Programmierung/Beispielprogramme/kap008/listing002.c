// kap008/listing002.c
#include <stdio.h>
#include <stdlib.h>
#define PI 3.1415

double Kflaeche( double d ) {
  return d * d * PI / 4;
}

double Kumfang( double d) {
  return d * PI;
}

int main(void) {
  double d = 0.0;
  printf("Kreisdurchmesser: ");
  if( scanf("%lf", &d) != 1 ) {
    printf("Fehler bei der Eingabe...\n");
   return EXIT_FAILURE;
  }
  printf("Kreisflaeche von %.2f = %.2f\n",d, Kflaeche(d));
  printf("Kreisumfang von %.2f  = %.2f\n",d, Kumfang(d));
  return EXIT_SUCCESS;
}
