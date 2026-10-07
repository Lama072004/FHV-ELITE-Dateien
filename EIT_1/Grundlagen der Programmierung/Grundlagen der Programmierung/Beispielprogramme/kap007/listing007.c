// kap007/listing007.c
#include <stdio.h>

long fakul( long n ) {
  if(n != 0 ) {
    return n * (fakul(n-1));
  }
  return 1;
}

int main(void) {
  printf("Fakultaet von 6: %ld\n", fakul(6));
  printf("Fakultaet von 8: %ld\n", fakul(8));
  return 0;
}
