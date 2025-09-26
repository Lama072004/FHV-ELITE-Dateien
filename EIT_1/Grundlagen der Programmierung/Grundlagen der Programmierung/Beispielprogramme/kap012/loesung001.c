// kap012/loesung001.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct artikel{
  char schlagzeile[255];
  int seite;
  int ausgabe;
} Artikel;

void output( Artikel *a ) {
  printf("%s\n", a->schlagzeile);
  printf("%d\n", a->seite);
  printf("%d\n\n", a->ausgabe);
}

int main(void) {
  Artikel art1 = {"Die Schlagzeile schlechthin",244, 33};
  Artikel *art2 = malloc(sizeof(Artikel));
  Artikel artArr[2];

  strncpy( art2->schlagzeile, "Eine Schlagzeile", 255);
  art2->seite = 212;
  art2->ausgabe = 43;

  strncpy( artArr[0].schlagzeile, "Noch eine", 255);
  artArr[0].seite = 266;
  artArr[0].ausgabe = 67;

  output( &art1 );
  output( art2 );
  output( &artArr[0] );
  return EXIT_SUCCESS;
}
