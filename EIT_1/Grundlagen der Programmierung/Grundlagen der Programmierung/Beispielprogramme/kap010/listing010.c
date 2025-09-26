// kap010/listing010.c
#include <stdio.h>
#include <stdlib.h>

double addition( double x, double y ) {
  return (x + y);
}

double subtraktion( double x, double y ) {
  return (x - y);
}

double multiplikation( double x, double y ) {
  return (x * y);
}

double division( double x, double y ) {
  return (x / y);
}

double (*fptr[4])(double d1, double d2) = {
  addition, subtraktion, multiplikation, division
};

int main(void) {
  double v1=0.0, v2=0.0;
  int operator=-1;
  printf("Zahl1 :-> ");
  if( scanf("%lf", &v1) != 1 ) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  printf("Zahl2 :-> ");
  if( scanf("%lf", &v2)  != 1 ) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  printf("Welcher Operator soll verwendet werden?\n");
  printf("0 = +\n1 = -\n2 = *\n3 = /\n");
  printf("Ihre Auswahl: ");
  if( scanf("%d", &operator)  != 1 ) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  if( ! (operator >= 0 && operator <= 3) ) {
    printf("Fehler beim Operator\n");
    return EXIT_FAILURE;
  }
  printf("Ergebnis: %lf\n", fptr[operator](v1, v2));
  return EXIT_SUCCESS;
}
