// kap003/listing011.c
#include <stdio.h>

int main(void) {
  int ivar = 1234;
  {  // Anweisungsblock - Anfang
    printf("%d\n", ivar);  // = 1234
  }  // Anweisungblock - Ende
  printf("%d\n", ivar);    // = 1234
  return 0;
}
