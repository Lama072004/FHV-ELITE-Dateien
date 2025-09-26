// kap004/listing006.c
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

int main(void) {
  long double ldval=8.8;
  double dval=5.5;
  float  fval=3.3;
  double complex dcval= 1.0 + 2.0*I;

  // Quadratwurzel mit reellen Zahlen
  printf("Quadratwurzel-Berechnungen:\n");
  printf("(long double) sqrt(%Lf) = %Lf\n"
                      ,ldval, sqrt(ldval));
  printf("(double) sqrt(%lf) = %lf\n",dval, sqrt(dval));
  printf("(float) sqrt(%f) = %f\n",fval, sqrt(fval));
  double complex dcval_G = sqrt(dcval);
  printf("(double complex) sqrt(4.0 + 2.0*I)"
    " = %lf + %lfi\n", creal(dcval_G), cimag(dcval_G));
  return 0;
}
