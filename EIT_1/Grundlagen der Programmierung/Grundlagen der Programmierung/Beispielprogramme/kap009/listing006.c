// kap009/listing006.c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int mdarray[2][3];
  // Array mit Werten initialisieren
  mdarray[0][0] = 12;
  mdarray[0][1] = 23;
  mdarray[0][2] = 34;
  mdarray[1][0] = 45;
  mdarray[1][1] = 56;
  mdarray[1][2] = 67;
  // Inhalt ausgeben
  for(int i=0; i < 2; i++) {
    for(int j=0; j < 3; j++ ) {
      printf("[%d][%d] = %d\n", i, j, mdarray[i][j]);
    }
  }
  return EXIT_SUCCESS;
}
