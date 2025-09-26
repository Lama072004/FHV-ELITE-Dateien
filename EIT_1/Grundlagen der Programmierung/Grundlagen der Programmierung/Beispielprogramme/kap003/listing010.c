// kap003/listing010.c
#include <stdio.h>

int main(void) {
  {  // Anweisungsblock - Anfang
    int ivar = 1234;
    printf("%d\n", ivar);
  }  // Anweisungblock - Ende
  printf("%d\n", ivar);  // Fehler !!!
  return 0;
}
