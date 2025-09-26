// kap006/listing002.c
#include <stdio.h>

int main(void) {
  for( int i=1, j=10; i < j; i++, j--) {
    printf("i=%d, j=%d\n", i, j);
  }
  return 0;
}
