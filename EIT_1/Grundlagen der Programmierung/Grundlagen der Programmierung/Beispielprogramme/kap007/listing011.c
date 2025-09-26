// kap007/listing011.c
#include <stdio.h>
int ival=789;

void funktion1(void) {
  printf("ival: %d\n", ival);
}

void funktion2(void) {
  int ival = 111;
  printf("ival: %d\n", ival);
}

int main(void) {
  int ival = 123;
  printf("ival: %d\n", ival);
  { // Neuer Anweisungsblock
    ival = 321;
    printf("ival: %d\n", ival);
  }
  printf("ival: %d\n", ival);
  { // Neuer Anweisungsblock
    int ival = 456;
    printf("ival: %d\n", ival);
  }
  printf("ival: %d\n", ival);
  funktion1();
  funktion2();
  return 0;
}
