// kap003/listing005.c
#include <stdio.h>
#include <limits.h>

int main(void) {
  printf("min. char-Wert     : %d\n", SCHAR_MIN);
  printf("max. char-Wert     : +%d\n", SCHAR_MAX);
  printf("min. short-Wert    : %d\n", SHRT_MIN);
  printf("max. short-Wert    : +%d\n", SHRT_MAX);
  printf("min. int-Wert      : %d\n", INT_MIN);
  printf("max. int-Wert      : +%d\n", INT_MAX);
  printf("min. long-Wert     : %ld\n", LONG_MIN);
  printf("max. long-Wert     : +%ld\n", LONG_MAX);
  printf("min. long long-Wert: %lld\n", LLONG_MIN);
  printf("max. long long-Wert: +%lld\n", LLONG_MAX);
  return 0;
}
