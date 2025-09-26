// kap009/loesung001.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  int iarr[]    = { 2, 4, 6, 4, 2, 4, 5, 6, 7 };
  double darr[] = { 3.3, 4.4, 2.3, 5.8, 7.7 };
  char str[]    = { "Hallo Welt"};

  printf("iarr: %zu Bytes\n", sizeof(iarr));
  printf("iarr: %zu Elemente\n", sizeof(iarr)/sizeof(int));
  printf("darr: %zu Bytes\n", sizeof(darr));
  printf("darr: %zu Elemente\n",sizeof(darr)/sizeof(double));
  printf("str : %zu Bytes\n", sizeof(str));
  printf("str : %zu Elemente (sizeof)\n",sizeof(str)/sizeof(char));
  printf("str : %zu Elemente (strlen)\n", strlen(str)+1);
  return EXIT_SUCCESS;
}
