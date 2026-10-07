// kap003/listing009.c
#include <stdio.h>
#include <limits.h>  // für CHAR_BIT
#include <assert.h>

static_assert( CHAR_BIT == 8,
               "unsigned char hat hier kein 8 Bit!" );


int main(void) {
  char cVal = 0;
  // ...
  printf("%d\n", cVal);
  return 0;
}
