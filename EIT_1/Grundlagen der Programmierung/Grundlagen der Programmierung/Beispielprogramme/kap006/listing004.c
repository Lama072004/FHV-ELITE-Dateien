// kap006/listing004.c
#include <stdio.h>

int main(void) {
  int auswahl = 0;
  do {
    printf("-1- Funktion 1 verwenden\n");
    printf("-2- Funktion 2 verwenden\n");
    printf("-3- Funktion 3 verwenden\n");
    printf("-0- Programm beenden\n\n");
    printf("Ihre Auswahl :\t");
    if( scanf("%d", &auswahl) != 1) {
      printf("Fehler bei der Eingabe...\n");
      return 1;
    }
    switch( auswahl ) {
      case 1 : printf("Funktion 1 bei der Arbeit\n");
               break;
      case 2 : printf("Funktion 2 bei der Arbeit\n");
               break;
      case 3 : printf("Funktion 3 bei der Arbeit\n");
               break;
      case 0 : printf("Beende Programm\n");
               break;
      default: printf("Falsche Eingabe!!!\n");
    }
 } while( auswahl != 0 );
 printf("Auf Wiedersehen!\n");
 return 0;
}
