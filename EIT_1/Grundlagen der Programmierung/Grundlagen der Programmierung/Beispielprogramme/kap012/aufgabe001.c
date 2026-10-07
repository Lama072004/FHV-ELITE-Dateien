// kap012/aufgabe001.c
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
  Artikel art1 = {244, "Die Schlagzeile schlechthin", 33};
  Artikel *art2;
  Artikel artArr[2];

  strncpy( art2->schlagzeile, "Eine Schlagzeile", 255);
  art2->seite = 212;
  art2->ausgabe = 43;

  strncpy( artArr.schlagzeile[0], "Noch eine", 255);
  artArr.seite[0] = 266;
  artArr.ausgabe[0] = 67;

  output( &art1 );
  output( art2 );
  output( &artArr[0] );
  return EXIT_SUCCESS;
}
