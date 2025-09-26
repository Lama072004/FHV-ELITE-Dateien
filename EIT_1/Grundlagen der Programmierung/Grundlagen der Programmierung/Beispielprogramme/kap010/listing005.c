// kap010/listing005.c
#include <stdio.h>
#include <stdlib.h>
#define MAX 5

int* ifget( void ) {
  static int puffer[MAX];
  for(int i=0; i<MAX; i++) {
    printf("Wert %d : ", i+1);
    if( scanf("%d", &puffer[i] ) != 1 ) {
      printf("Fehler bei der Eingabe\n");
      return EXIT_SUCCESS;
    }
  }
  return puffer;
}

int main(void) {
  int* iptr = ifget( );
  printf("Folgende Werte wurden eingelesen\n");
  for(int i=0; i < MAX; i++ ) {
    printf("%d : %d\n", i+1, *(iptr+i));
  }
  return EXIT_SUCCESS;
}
