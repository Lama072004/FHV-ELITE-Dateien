// kap010/listing002.c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *iptr;
  int ival = 255;
  iptr = &ival;
  // Wert ausgeben
  printf("*iptr  :  %d\n", *iptr);
  printf(" ival  :  %d\n", ival);
  // ival neuen Wert zuweisen
  *iptr = 128;
  // Wert ausgeben
  printf("*iptr  :  %d\n", *iptr);
  printf(" ival  :  %d\n", ival);
  return EXIT_SUCCESS;
}
