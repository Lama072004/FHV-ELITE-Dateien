// kap009/listing003.c
#include <stdio.h>
#include <stdlib.h>
#if __STDC_NO_VLA__
  #error "No VLA support!"
#endif

int main(void) {
  int val = 0;
  printf("Anzahl der Elemente: ");
  if( scanf("%d", &val) != 1 ) {
    printf("Fehler bei der Eingabe...\n");
    return EXIT_FAILURE;
  }
  if(val > 0) {
    int iarr[val];
    for(int i = 0; i < val; i++) {
      iarr[i] = i;
    }
    for(int i = 0; i < val; i++) {
      printf("%d\n", iarr[i]);
    }
  }
  return EXIT_SUCCESS;
}
