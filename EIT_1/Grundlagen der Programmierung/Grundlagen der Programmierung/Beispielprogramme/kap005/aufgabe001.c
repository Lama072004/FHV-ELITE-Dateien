// kap005/aufgabe001.c
#include <stdio.h>

int main(void) {
  int work = 0;
  printf("-1- PC 1 hochfahren\n");
  printf("-2- PC 2 hochfahren\n");
  printf("-3- Drucker einschalten\n");
  printf("-4- Kaffee machen\n");
  printf("-5- Feierabend machen\n");
  printf("Was wollen Sie tun: ");
  if( scanf("%d", &work) != 1 ) {
    printf("Fehler bei der Eingabe...\n");
    return 1;
  }
  if( work == 1 ) {
    printf("PC 1 wird hochgefahren\n");
  }
  else if( work == 2 ) {
    printf("PC 2 wird hochgefahren\n");
  }
  else if( work == 3 ) {
    printf("Drucker wird eingeschaltet\n");
  }
  else if( work == 4 ) {
    printf("Kaffee wird gemacht\n");
  }
  else if( work == 5 ) {
    printf("Gute Nacht\n");
  }
  else {
    printf("Falsche Eingabe !\n");
  }
  return 0;
}
