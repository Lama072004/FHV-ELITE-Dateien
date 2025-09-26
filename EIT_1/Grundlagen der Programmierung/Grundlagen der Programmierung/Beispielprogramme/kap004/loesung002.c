// kap004/loesung002.c
#include <stdio.h>

int main(void) {
  double cel = 0.0;
  printf("Temperatur in Celsius: ");
  int check = scanf("%lf", &cel );
  if( check != 1 ) {
    printf("Fehler bei der Eingabe...\n");
    return 1;
  }
  double kelvin = cel + 273.15;
  double fahrenheit = (cel*9)/5+32;
  printf("%lf Grad Celsius sind ...\n", cel);
  printf("%lf Kelvin\n", kelvin);
  printf("%lf Grad Fahrenheit\n", fahrenheit);
  return 0;
}
