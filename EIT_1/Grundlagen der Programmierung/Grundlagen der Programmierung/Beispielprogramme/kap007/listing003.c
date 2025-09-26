// kap007/listing003.c
#include <stdio.h>

void hallo(void);  // Funktionsprototyp

int main(void) {
  printf("Vor der Funktion\n");
  hallo();  // Funktionsaufruf
  printf("Nach der Funktion\n");
  return 0;
}

// Funktionsdefinition
void hallo(void) {
  printf("In der Funktion\n");
}
