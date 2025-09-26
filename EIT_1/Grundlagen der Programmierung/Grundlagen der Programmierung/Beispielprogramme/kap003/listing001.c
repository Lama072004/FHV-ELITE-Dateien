// kap003/listing001.c
#include <stdio.h>

int main(void) {
  signed char cVal = 100;
  short sVal = 10000;
  int iVal = 123456;
  long lVal = 123456;
  long long llVal = 123456;

  printf("%d\n", cVal);
  printf("%d\n", sVal);
  printf("%d\n", iVal);
  printf("%ld\n", lVal);
  printf("%lld\n", llVal);
  return 0;
 }
