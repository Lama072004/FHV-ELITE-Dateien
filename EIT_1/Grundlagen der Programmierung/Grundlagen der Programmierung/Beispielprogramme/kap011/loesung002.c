// kap011/loesung002.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BLK 64

void cmp_adress(const int *adr1, const int *adr2 ) {
  if( adr1 != adr2 ) {
    printf("realloc musste umkopieren\n");
    printf("Alte Adresse: %p\n", adr1);
    printf("Neue Adresse: %p\n", adr2);
  }
}

int main(void) {
  size_t len = BLK;
  int val = 0;
  int* ivals = malloc(BLK * (sizeof(int)));
  if(NULL == ivals) {
    printf("Kein virtueller RAM mehr vorhanden ... !");
    return EXIT_FAILURE;
  }
  int* iptr = ivals;
  while(1) {
    printf("Wie viel neuer Speicher > ");
    if( scanf("%d", &val) != 1 ) {
      printf("Fehler bei der Eingabe\n");
      return EXIT_FAILURE;
    }
    // Aktuelle Länge von str zählen für realloc
    len += val;
    // Neuen Speicher für str anfordern
    ivals = realloc(ivals,len);
    if(NULL == ivals) {
      printf("Kein virtueller RAM mehr vorhanden ... !");
      return EXIT_FAILURE;
    }
    cmp_adress( ivals, iptr );
    iptr = ivals;
  }
  return EXIT_SUCCESS;
}
