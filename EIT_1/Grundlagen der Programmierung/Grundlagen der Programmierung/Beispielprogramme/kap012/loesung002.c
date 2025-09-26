// kap012/loesung002.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct artikel{
  char schlagzeile[255];
  int seite;
  int ausgabe;
} Artikel;

int ArtikelCmp( Artikel *art1, Artikel *art2 ) {
   if( strcmp(art1->schlagzeile, art2->schlagzeile) ) {
     return 1;
   }
   else if( art1->seite != art2->seite ) {
     return 1;
   }
   else if( art1->ausgabe != art2->ausgabe ) {
     return 1;
   }
   return 0;
}

int main(void) {
  Artikel art1 = {"Die Schlagzeile schlechthin", 244, 33};
  Artikel art2 = {"Die Schlagzeile schlechthin", 244, 33};
  Artikel art3 = {"Die Schlagzeile_schlechthin", 244, 33};

  if( ArtikelCmp( &art1, &art2 ) ) {
     printf("art1 und art2 sind nicht gleich\n");
  }
  if( ArtikelCmp( &art2, &art3 ) ) {
     printf("art2 und art3 sind nicht gleich\n");
  }
  return EXIT_SUCCESS;
}
