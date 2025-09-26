// kap008/loesung002.c
#include <stdio.h>
#include <stdlib.h>
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

int main(void) {
  int val1 = 20, val2 = 30;
  printf("Max. Wert: %d\n", MAX(val1, val2) );
  printf("Min. Wert: %d\n", MIN(val1, val2) );
  return EXIT_SUCCESS;
}
