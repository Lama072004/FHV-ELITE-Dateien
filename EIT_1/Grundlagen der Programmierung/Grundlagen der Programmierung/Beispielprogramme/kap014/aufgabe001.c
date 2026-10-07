// kap014/aufgabe001.c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int i = 0;
  char filename[255];
  FILE *fp[1000];
  printf("Programmstart\n");
  while( i < 1000 ) {
    // Dateinamen basteln mit fortlaufender Nummer
    sprintf( filename, "Berechnung%d.txt", i);
    // Umfangreiche Berechnung hier
    // ...
    // Ergebnis in eine Datei
    fp[i] = fopen( filename, "w" );
    if( fp[i] != NULL ) {
      fprintf(fp[i], "Ergebnis der Berechnung Nr.%d", i );
    }
    else {
      fprintf(stderr, "Ergebnis der Berechnung Nr.%d", i);
    }
    ++i;
  }
  return EXIT_SUCCESS;
}
