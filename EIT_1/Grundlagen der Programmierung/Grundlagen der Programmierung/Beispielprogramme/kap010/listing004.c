// kap010/listing004.c
#include <stdio.h>
#include <stdlib.h>

void reset( int *val ) {
  *val = 0;
}

int main(void) {
  int ival = 1234567;
  printf("ival: %d\n", ival);   // = 1234567
  reset( &ival );
  printf("ival: %d\n", ival);   // = 0
  return EXIT_SUCCESS;
}
