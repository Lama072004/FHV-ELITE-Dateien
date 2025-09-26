// kap009/listing009.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 50

void nl2space( char str[] ) {
  int n = strlen(str);
  for(int i = 0; i < n; i++) {
    if( str[i] == '\n' ) {
      str[i] = ' ';
    }
  }
}

void nl2null( char str[] ) {
  int n = strlen(str)-1;
  if(str[n] == '\n') {
    str[n] = '\0';
  }
}

int main(void) {
  char name[MAX*2];
  char vname[MAX], nname[MAX];
  printf("Vorname : ");
  if( fgets(vname, MAX, stdin) == NULL ) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  nl2space( vname );
  printf("Nachname: ");
  if( fgets(nname, MAX, stdin) == NULL ) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  nl2null( nname );

  // Strings vergleichen
  if( strncmp( vname, nname, MAX ) == 0) {
    printf("Vorname und Nachname sind identisch\n");
    return EXIT_FAILURE;
  }
  // vname nach name kopieren
  if( strncpy(name, vname, MAX) == NULL ) {
    printf("Fehler bei strncpy\n");
    return EXIT_FAILURE;
  }
  // noch vorhandenen Platz in name ermitteln
  size_t len = MAX*2 - strlen(name)+1;
  // nname an name anhängen
  if( strncat(name, nname, len) == NULL ) {
    printf("Fehler bei strncat\n");
    return EXIT_FAILURE;
  }
  // gesamten String ausgaben
  printf("Ihr Name: %s\n", name);
  return EXIT_SUCCESS;
}
