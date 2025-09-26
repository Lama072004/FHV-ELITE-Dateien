// kap005/listing006.c
#include <stdio.h>

int main(void) {
  int eingabe = 0;
  printf("-1- Level 1\n");
  printf("-2- Level 2\n");
  printf("-3- Level 3\n");
  printf("-4- Beenden\n");
  printf("Ihre Auswahl bitte: ");
  int check = scanf("%d", &eingabe);
  if( check != 1) {
    printf("Fehler bei der Eingabe...\n");
    return 1;
  }
  switch(eingabe) {
    case 1  : printf("Level 1 war die Auswahl\n");
              break;
    case 2  : printf("Level 2 war die Auswahl\n");
              break;
    case 3  : printf("Level 3 war die Auswahl\n");
              break;
    case 4  : printf("Beenden war die Auswahl\n");
              break;
    default : printf("%d? Unbekanntes Level!\n", eingabe);
  }
  return 0;
}
