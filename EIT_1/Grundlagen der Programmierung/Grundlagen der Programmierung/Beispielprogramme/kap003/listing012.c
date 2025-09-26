// kap003/listing012.c
#include <stdio.h>

int main(void) {
  int ivar = 1234;
  {  // Anweisungsblock – Anfang
    int ivar = 4321;
    printf("%d\n", ivar);  // = 4321
  }  // Anweisungblock - Ende
  printf("%d\n", ivar);    // = 1234
  return 0;
}
