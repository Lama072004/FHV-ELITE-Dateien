// kap007/listing002.c
#include <stdio.h>

int main(void) {
  printf("Vor der Funktion\n");
  hallo();  // Fehler/Warnung: hallo() unbekannt
  printf("Nach der Funktion\n");
  return 0;
}

void hallo(void) {
  printf("In der Funktion\n");
}
