// kap011/listing002.c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  unsigned int val=0;
  printf("Wie viele int-Elemente benoetigen Sie: ");
  if( scanf("%u", &val) != 1 ) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  int* arr = calloc( val, (sizeof(int) ) );
  if( arr == NULL ) {
    printf("Fehler bei der Speicherreservierung!\n");
    return EXIT_FAILURE;
  }
  printf("Ausgabe der Elemente\n");
  for(unsigned int i=0; i < val; i++ ) {
    printf("arr[%u] = %u\n", i, arr[i]);
  }
  if( arr != NULL ) {
    free(arr);
  }
  return EXIT_SUCCESS;
}
