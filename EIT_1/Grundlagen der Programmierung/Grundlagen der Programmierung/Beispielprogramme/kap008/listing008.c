// kap008/listing008.c
#include <stdio.h>
#include <complex.h>
#include <stdlib.h>

#define genfunc(X) _Generic ((X),  \
  default: genfunc_integer,       \
  double: genfunc_double,         \
  double complex: genfunc_complex \
)(X)

void genfunc_integer(int i) {
  printf("Integer: %d\n", i);
}

void genfunc_double(double d) {
  printf("Double: %lf\n", d);
}

void genfunc_complex(double complex c) {
  printf("Complex: %lf %lf\n", creal(c), cimag(c));
}

int main(void) {
  int ival = 12345;
  double dval = 3.14;
  double complex dc = 2.2 + 3.3 * I;
  genfunc(ival); // verwendet genfunc_integer
  genfunc(dval); // verwendet genfunc_double
  genfunc(dc);   // verwendet genfunc_complex
  return EXIT_SUCCESS;
}
