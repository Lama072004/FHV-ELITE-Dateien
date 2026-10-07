// kap014/listing009.c
#include <stdio.h>
#include <stdlib.h>
#define FILENAME "listing009.c" // Anpassen

void dump_buffer(FILE *fp) {
  int ch;
  while( (ch = fgetc(fp)) != EOF && ch != '\n' )
  /* Kein Anweisungsblock nötig */ ;
}

long fileSize( FILE *fp ) {
  if( fseek( fp, 0L, SEEK_END ) != 0 ) {
    fprintf(stderr, "Fehler bei fseek\n");
    return -1;
  }
  return ftell(fp);
}

int main (void) {
  long endPos = 0, aktPos = 0, neuPos = 0;
  FILE *rfp = fopen(FILENAME, "r");
  if( rfp != NULL ) {
    endPos = fileSize( rfp );
    if(endPos == -1 ) {
      return EXIT_FAILURE;
    }
  }
  else {
    fprintf(stderr, "Fehler beim Oeffnen\n");
    return EXIT_FAILURE;
  }
  rewind( rfp ); // Wieder zum Anfang zurück
  printf("Lesezeiger nach vorne setzen. Um wie viel: ");
  if (scanf("%ld", &neuPos) != 1 ) {
    fprintf(stderr, "Fehler bei der Eingabe\n");
    return EXIT_SUCCESS;
  }
  dump_buffer(stdin);
  if( neuPos > endPos ) {
    fprintf(stderr, 
      "Fehler: Datei ist nur %ld Byte groß\n",endPos);
  }
  if( fseek( rfp, neuPos, SEEK_CUR ) != 0 ) {
    fprintf(stderr, "Fehler bei fseek\n");
    return EXIT_FAILURE;
  }
  int c;
  while( (c=fgetc(rfp)) != EOF ) {
    fputc(c, stdout);
  }
  fclose(rfp);
  return EXIT_SUCCESS;
}
