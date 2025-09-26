// kap010/loesung002.c
#include <stdio.h>
#include <stdlib.h>

double Vquader(const double *a, const double *b, const double *c) {
  return ((*a) * (*b) * (*c));
}

int main(void) {
  double d1 = 6.0, d2=4.1, d3=3.2;
  printf("Quadervolumen: %lf\n", Vquader(&d1,&d2,&d3));
  return EXIT_SUCCESS;
}
