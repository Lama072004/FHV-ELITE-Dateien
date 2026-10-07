// kap014/listing001.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAME 1024

int main(void) {
  char filename[NAME];
  printf("Welche Datei soll geoeffnet werden: ");
  if( fgets(filename, sizeof(filename), stdin) == NULL ) {
    fprintf(stderr, "Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  // Newline entfernen
  size_t p = strlen(filename);
  filename[p-1] = '\0';
  FILE *fp = fopen(filename, "r");
  if( fp != NULL ) {
    printf("Datei zum Lesen geoeffnet\n");
    fclose(fp);
  }
  else {
    fprintf(stderr,"Datei konnte nicht geoeffnet werden");
  }
  return EXIT_SUCCESS;
}
