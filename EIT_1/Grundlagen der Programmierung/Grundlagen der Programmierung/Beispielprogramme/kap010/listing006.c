// kap010/listing006.c
#include <stdio.h>
#include <stdlib.h>
#define MAX 5

int main(void) {
  int iarr[MAX] = { 12, 34, 56, 78, 90 };
  int *iptr = NULL;
  // iptr zeigt auf das erste Element von iarr
  iptr = iarr;

  printf("iarr[0] = %d\n", *iptr);
  printf("iarr[2] = %d\n", *(iptr+2));
  *(iptr+4) = 66;   // iarr[4] = 66
  *iptr = 99;       // iarr[0] = 99
  // Alle auf einmal durchlaufen (2 Möglichkeiten)
  int *iptr2 = iarr;  // iptr2 auf den Anfang von iarr
  for(int i=0; i < MAX; i++, iptr2++) {
    printf("iarr[%d] = %d /", i, *(iptr+i));
    printf(" %d\n", *iptr2);  // so geht es auch
  }
  return EXIT_SUCCESS;
}
