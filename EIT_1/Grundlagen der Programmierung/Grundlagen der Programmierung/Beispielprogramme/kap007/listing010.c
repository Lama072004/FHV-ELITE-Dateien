// kap007/listing010.c
#include <stdio.h>

int main(void) {
  int ival = 123;
  printf("ival: %d\n", ival);
  { // Neuer Anweisungsblock
    ival = 321;
    printf("ival: %d\n", ival);
  }
  printf("ival: %d\n", ival);
  { // Neuer Anweisungsblock
    int ival = 456;
    printf("ival: %d\n", ival);
  }
  printf("ival: %d\n", ival);
  return EXIT_SUCCESS;
}
