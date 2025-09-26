// kap009/listing008.c
#include <stdio.h>
#include <stdlib.h>
#define MAX 20

int main(void) {
  char string1[MAX];
  printf("Eingabe machen : ");
  if (fgets(string1, sizeof(string1), stdin) == NULL ) {
    printf("Fehler beim Einlesen\n");
    return EXIT_FAILURE;
  }
  printf("Ihre Eingabe: %s", string1);
  return EXIT_SUCCESS;
}
