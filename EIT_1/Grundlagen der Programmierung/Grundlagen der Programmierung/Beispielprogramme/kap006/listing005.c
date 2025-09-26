// kap006/listing005.c
#include <stdio.h>

int main(void) {
  int ival = 0;
  while( 1 ) {
    printf("Raus geht es mit 5 : ");
    if( scanf("%d", &ival ) != 1 ) {
      printf("Fehler bei der Eingabe...\n");
      break;
    }
    if( ival == 5 ) {
      break;
    }
  }
  printf("Programmende\n");
  return 0;
}
