// kap014/loesung001.c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int i = 0;
  char filename[255];
  FILE *fp = NULL;
  printf("Programmstart\n");
  while( i < 1000 ) {
    sprintf( filename, "Berechnung%d.txt", i);
    fp = fopen( filename, "w" );
    if( fp != NULL ) {
      fprintf(fp, "Ergebnis der Berechnung Nr.%d", i );
      fclose(fp);
    }
    else {
      fprintf(stderr, "Ergebnis der Berechnung Nr.%d", i);
    }
    ++i;
  }
  return EXIT_SUCCESS;
}
