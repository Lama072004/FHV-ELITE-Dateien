// kap007/listing008.c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int ival = 0;
  printf("Ganzzahl eingeben: ");
  if(scanf("%d", &ival) != 1) {
    printf("Fehler bei der Eingabe!\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
