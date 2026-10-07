// kap010/listing003-null.c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *iptr = NULL; // Zeiger mit NULL initialisiert
  // ... mehr Code
  if( iptr == NULL ) {
    printf("Zeiger hat keine gueltige Adresse\n");
    return EXIT_FAILURE;
  }
  // iptr hat eine gültige Adresse ...
  return EXIT_SUCCESS;
}
