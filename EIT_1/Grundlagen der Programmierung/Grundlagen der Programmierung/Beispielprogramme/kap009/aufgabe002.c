// kap009/aufgabe002.c
#include <stdio.h>
#include <stdlib.h>
#define MAX 10

int main(void) {
  char v[5] = { 'A', 'E', 'I', 'O', 'U' };
  printf("Die einzelnen Vokale: ");
  for(int i=0; i < 5; i++ ) {
    printf("%c (Dezimal: %d)\n", v[i], v[i]);
  }
  printf("Alle zusammen: %s\n", v);
  return EXIT_SUCCESS;
}
