// kap014/listing008.c
#include <stdio.h>
#include <stdlib.h>

int main (void) {
  float fval = 3.14132;
  char text[] = "Tiefstand";
  printf("|%08.2f|\n", fval);
  printf("|%-8.0f|\n", fval);
  printf("|%8.4s|\n", text);
  printf("|%-8.4s|\n", text);
  return EXIT_SUCCESS;
}
