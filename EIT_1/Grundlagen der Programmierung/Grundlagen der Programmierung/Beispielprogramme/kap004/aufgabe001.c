// kap004/aufgabe001.c
#include <stdio.h>

int main(void) {
  int iVar = 0;
  printf("Bitte eine Ganzzahl eingeben: ");
  int check = scanf("%d", iVar);

  if( check != 1 ) {
    printf("Fehler bei scanf...\n");
    return 1;  // Programm beenden
  }
  printf("%d Wert(e) eingelesen; ", check);
  printf("der eingegebene Wert lautet: %d\n", iVar);
  return 0;
}
