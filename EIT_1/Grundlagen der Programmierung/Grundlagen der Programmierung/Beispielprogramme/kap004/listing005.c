// kap004/listing005.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h> // C99

int main(void) {
  long double ldval=8.8;
  double dval=5.5;
  float  fval=3.3;

  // Quadratwurzel mit reellen Zahlen
  printf("Quadratwurzel-Berechnungen:\n");
  printf("(long double) sqrtl(%Lf) = %Lf\n",
                         ldval, sqrtl(ldval));
  printf("(double) sqrt(%lf) = %lf\n",dval,sqrt(dval));
  printf("(float) sqrtf(%f) = %f\n",fval,sqrtf(fval));
  // Berechnung mit komplexen Zahlen
 double pi = 4 * atan(1.0);
 double complex c = cexp(I * pi);
 printf("%lf + %lf * i\n", creal(c), cimag(c));
 return 0;
}
