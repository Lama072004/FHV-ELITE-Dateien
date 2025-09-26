// kap005/listing001.c
#include <stdio.h>

int main(void) {
  int ival = 0;
  printf("Bitte eine Ganzzahl eingeben: ");
  int check = scanf("%d", &ival);
  if( check ) {
    // Der Code wird ausgeführt, wenn die Bedingung, dass
    // der Wert check nicht 0 ist, wahr ist.
     printf("Ihre Eingabe: %d\n", ival);
  }
  printf("Ausserhalb der if-Verzweigung\n");
  return 0;
}
