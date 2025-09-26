// kap007/listing001.c
#include <stdio.h>

void hallo(void) {
  printf("In der Funktion\n");
}

int main(void) {
  printf("Vor der Funktion\n");
  hallo();  // Funktionsaufruf
  printf("Nach der Funktion\n");
  return 0;
}
