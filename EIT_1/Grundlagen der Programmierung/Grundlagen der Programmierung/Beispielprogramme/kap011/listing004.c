// kap011/listing004.c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  double *dptr1=NULL, *dptr2=NULL;
  dptr1 = calloc( 10, sizeof(double) );
  dptr2 = calloc( 20, sizeof(double) );
  // dptr1 verweist auf dieselbe Adresse wie dptr2
  dptr1 = dptr2;          // Speicherleck erstellt
  //... nach vielen Zeilen Code, Speicher freigeben
  free( dptr1 );  // Gibt Speicher frei
  free( dptr2 );  // Fehler !!!!
  return EXIT_SUCCESS;
}
