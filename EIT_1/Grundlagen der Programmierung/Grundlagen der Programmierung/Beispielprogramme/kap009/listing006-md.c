// kap009/listing006-md.c
#include <stdio.h>
#include <stdlib.h>
#define WOCHE 2
#define TAGE 7

void durchschnitt( int arr[][TAGE] ) {
  int durchs=0;
  for( int i=0; i < WOCHE; i++) {
    for( int j=0; j < TAGE; j++) {
      durchs+=arr[i][j];
    }
  }
  printf("Besucher in %d Tagen\n", WOCHE*TAGE);
  printf("Gesamt : %d\n", durchs);
  printf("Tagesschnitt: %d\n", durchs /(WOCHE*TAGE));
}

int main(void) {
  int besucher[WOCHE][TAGE];
  printf("Besucherzahlen eingeben\n\n");
  for(int i=0; i < WOCHE; i++) {
    for(int j=0; j < TAGE; j++) {
      printf("Woche %d, Tag %d: ", i+1, j+1);
      if( scanf("%d", &besucher[i][j]) != 1 ) {
        printf("Fehler bei der Eingabe\n");
        return EXIT_FAILURE;
      }
    }
  }
  durchschnitt( besucher );
  return EXIT_SUCCESS;
}
