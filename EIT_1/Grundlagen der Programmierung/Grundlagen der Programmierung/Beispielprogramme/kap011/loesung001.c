// kap011/loesung001.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BUF 4096

int main(void) {
  size_t len;
  char *str = NULL;
  char puffer[BUF+1];

  printf("Text eingeben\n> ");
  if( fgets(puffer, sizeof(puffer), stdin) == NULL ) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  str = malloc(strlen(puffer)+1);
  if(NULL == str) {
    printf("Kein virtueller RAM mehr vorhanden ... !");
    return EXIT_FAILURE;
  }
  strcpy(str, puffer);
  while(1) {
    printf("Weiterer Text (Beenden mit ENDE)\n>");
    if( fgets(puffer, sizeof(puffer), stdin) == NULL ) {
      printf("Fehler bei der Eingabe\n");
      return EXIT_FAILURE;
    }
    // Abbruchbedingung
    if( strcmp(puffer,"ende\n")==0 ||
      strcmp(puffer,"ENDE\n")==0) {
      break;
    }
    // Aktuelle Länge von str zählen für realloc
    len = strlen(str);
    // Neuen Speicher für str anfordern
    str = realloc(str,strlen(puffer)+len+1);
    if(NULL == str) {
      printf("Kein virtueller RAM mehr vorhanden ... !");
      return EXIT_FAILURE;
    }
    // Hinten anhängen
    strcat(str, puffer);
  }
  printf("Der gesamte Text lautet (%zd Bytes): \n", sizeof(str));
  printf("%s", str);
  free(str);
  return EXIT_SUCCESS;
}
