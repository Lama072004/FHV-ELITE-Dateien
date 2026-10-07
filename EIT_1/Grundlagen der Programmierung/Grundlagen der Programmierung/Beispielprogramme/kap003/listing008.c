// kap003/listing008.c
#include <stdio.h>
#include <assert.h>

static_assert( sizeof(long double) == 16,
               "Need 16 byte long double" );

int main(void) {
  long double ldVal = 123.123L;
  // ...
  return 0;
}
