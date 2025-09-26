// kap011/listing003.c
#include <stdio.h>
#include <stdlib.h>
#define BLKSIZE 8

int main(void) {
  int n=0, max=BLKSIZE, z=0;
  int* zahlen = calloc(BLKSIZE, sizeof(int));
  if(NULL == zahlen) {
    printf("Kein virtueller RAM mehr vorhanden!\n");
    return EXIT_FAILURE;
  }
  printf("Zahlen eingeben --- Beenden mit 0\n");
  while(1) {
    printf("Zahl (%d) eingeben : ", n+1);
    if( scanf("%d", &z) != 1 ) {
      printf("Fehler bei der Eingabe\n");
      return EXIT_FAILURE;
    }
    if(z==0) { break; } // Schleifenabbruch
    if(n >= max-1) {
      max += BLKSIZE;
      zahlen = realloc(zahlen, max * sizeof(int) );
      if(NULL == zahlen) {
        printf("Kein virtueller RAM mehr vorhanden!");
        return EXIT_FAILURE;
      }
      printf("Neuer Speicher: %d Bytes\n", BLKSIZE);
      printf("Insgesamt    : %zd Bytes\n",
         sizeof(int)*max);
      printf("Platz fuer   : %d Elemente\n", max);
    }
    zahlen[n++] = z;
  }
  printf("Folgende Zahlen wurden eingegeben ->\n\n");
  for(int i = 0; i < n; i++) {
    printf("%d ", zahlen[i]);
  }
  printf("\n");
  free(zahlen);
  return EXIT_SUCCESS;
}
