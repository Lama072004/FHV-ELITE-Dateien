// kap009/loesung002.c
#include <stdio.h>
#include <stdlib.h>
#define EQUAL -1
#define DIFFSIZE -2

int vergleich( int arr1[], int n1, int arr2[], int n2 ) {
  if(n1 != n2) {
    return DIFFSIZE; // Arrays unterschiedlich lang
  }
  for(int i=0; i < n1; i++) {
    if( arr1[i] != arr2[i] ) {
      return i; // Indexnummer mit Unterschied
    }
  }
  return EQUAL; // Beide Arrays sind identisch
}

int main(void) {
  int iarr1[] = { 2, 1, 4, 5, 6, 2, 1 };
  int iarr2[] = { 2, 1, 4, 6, 6, 2, 1 };
  int ret = vergleich( iarr1,(sizeof(iarr1)/sizeof(int)),
                       iarr2,(sizeof(iarr2)/sizeof(int)));
  if( ret == DIFFSIZE ) {
    printf("Die Arrays sind unterschiedlich lang\n");
  }
  else if( ret == EQUAL ) {
    printf("Beide Arrays sind identisch\n");
  }
  else {
    printf("Unterschied an Pos. %d gefunden\n", ret);
  }
  return EXIT_SUCCESS;
}
