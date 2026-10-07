// kap006/listing003.c
#include <stdio.h>

int main(void) {
  int ival = 10;
  while( ival > 0 ) {
    if( ival % 2 ) {
      printf("%d ", ival);
    }
    ival--;
  }
  printf("\n");
  return 0;
}
