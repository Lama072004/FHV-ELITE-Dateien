// kap003/listing003.c
#include <stdio.h>
#include <complex.h>

int main(void) {
  float complex fc = 2.0 + 3.0*I;
  // 4 Bytes
  printf("sizeof(float) : %zu\n", sizeof(float));
  // 8 Bytes (realer und imaginärer Teil)
  printf("sizeof(float complex) : %zu\n",
                 sizeof(float complex));
  // Ausgabe vom Real- und Imaginärteil
  printf("%f + %f\n", creal(fc), cimag(fc));
  return 0;
}
