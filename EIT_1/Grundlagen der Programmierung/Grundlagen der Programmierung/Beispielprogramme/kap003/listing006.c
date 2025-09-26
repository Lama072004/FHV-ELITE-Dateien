// kap003/listing006.c
#include <stdio.h>
#include <float.h>

int main(void) {
  printf("float Genauigkeit       : %d\n", FLT_DIG);
  printf("double Genauigkeit      : %d\n", DBL_DIG);
  printf("long double Genauigkeit : %d\n", LDBL_DIG);
  return 0;
}
