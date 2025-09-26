// kap007/loesung003.c
#include <stdio.h>

long fakul( long n ) {
  long val = n;
  while(--val ) {
    n*=val;
  }
  return n;
}

int main(void) {
  printf("Fakultät von 6: %ld\n", fakul(6));
  printf("Fakultät von 8: %ld\n", fakul(8));
  return 0;
}
