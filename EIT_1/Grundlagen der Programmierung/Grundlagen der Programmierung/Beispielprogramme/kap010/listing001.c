// kap010/listing001.c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *iptr;
  int ival = 255;
  iptr = &ival;
  printf("Adresse iptr: %p\n", &iptr);
  printf("zeigt auf   : %p\n", iptr);
  printf("Adresse ival: %p\n", &ival);
  return EXIT_SUCCESS;
}
