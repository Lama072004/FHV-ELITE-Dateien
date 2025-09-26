// kap005/listing005.c
#include <stdio.h>

int main(void) {
  int ival = 0;
  printf("Bitte eine positive Ganzzahl eingeben: ");
  int check = scanf("%d", &ival);
  if( check == 1 ) {
    if(ival <= 0) {
      printf("Keine negativen Werte oder 0 verwenden\n");
    }
    else {
      printf("Ihre Eingabe: %d\n", ival);
    }
  }
  else {
    printf("Fehler bei der Eingabe!\n");
  }
  printf("Ausserhalb der if-Verzweigung\n");
  return 0;
}
