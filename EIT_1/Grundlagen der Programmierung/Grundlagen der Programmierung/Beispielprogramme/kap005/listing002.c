// kap005/listing002.c
#include <stdio.h>

int main(void) {
  int ival = 0;
  printf("Bitte eine Ganzzahl eingeben: ");
  int check = scanf("%d", &ival);
  if( check == 1 ) {
    printf("Ihre Eingabe: %d\n", ival);
  }
  else {
    printf("Fehler bei der Eingabe!\n");
  }
  printf("Ausserhalb der if-Verzweigung\n");
  return 0;
}
