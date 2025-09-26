// kap007/listing009.c
#include <stdio.h>
#include <stdlib.h>

int readint(void) {
  int val = 0;
  printf("Bitte eine Ganzzahl eingeben: ");
  if(scanf("%d", &val) != 1) {
    printf("Fehler bei der Eingabe!\n");
    exit(EXIT_FAILURE);
  }
  return val;
}

int main(void) {
  int ival = readint();
  printf("Der Wert lautet: %d\n", ival);  
  return EXIT_SUCCESS;
}
