// kap008/listing003.c
#include <stdio.h>
#include <stdlib.h>
#define NOT_EQUAL(x, y)   ((x) != (y))
#define XCHANGE(x, y) { \
   int j; j=(x); (x)=(y); (y)=j; }

int main(void) {
  int ival1 = 0, ival2 = 0;
  printf("Bitte zwei Ganzzahlwerte eingeben: ");
  if( scanf("%d %d", &ival1, &ival2) != 2 ) {
    printf("Fehler bei der Eingabe...\n");
    return EXIT_FAILURE;
  }
  if( NOT_EQUAL(ival1, ival2) ) {
    XCHANGE(ival1, ival2);
  }
  printf("val1: %d - val2: %d\n", ival1, ival2);
  return EXIT_SUCCESS;
}
