// kap014/listing003.c
#include <stdio.h>
#include <stdlib.h>
#define FILENAME "listing003.c" // Anpassen
#define COPY "listing003.bak"   // Anpassen

int main(void) {
  FILE *fpr = fopen( FILENAME, "r" );
  if( fpr == NULL ) {
    fprintf(stderr, "Fehler beim Oeffnen: %s\n", FILENAME);
    return EXIT_FAILURE;
  }
  FILE *fpw = fopen( COPY, "w" );
  if( fpw == NULL ) {
    fprintf(stderr, "Fehler beim Oeffnen: %s\n", COPY);
    return EXIT_FAILURE;
  }
  int c;
  while ( (c=fgetc(fpr)) != EOF ) {
    if( c > 127 ) {
      fputc('-', stdout);
    }
    else {
      fputc(c, stdout);
    }
    if( fputc(c, fpw ) == EOF ) {
      fprintf(stderr, "Fehler beim Schreiben\n");
      break;
    } 
  }
  if( c == EOF ) {
    if( feof(fpr) ) {
      printf("Dateiende erreicht\n");
    }
    else if( ferror(fpr) ) {
      printf("Ein Fehler beim Lesen ist aufgetreten!\n");
    }
  }
  fclose(fpr);
  fclose(fpw);
  return EXIT_SUCCESS;
}
