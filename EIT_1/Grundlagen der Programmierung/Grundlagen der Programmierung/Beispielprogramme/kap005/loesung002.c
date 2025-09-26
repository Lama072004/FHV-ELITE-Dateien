// kap005/loesung002.c
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
  switch( work ) {
    case 1 : printf("PC 1 wird hochgefahren\n");
             break;
    case 2 : printf("PC 2 wird hochgefahren\n");
             break;
    case 3 : printf("Drucker wird eingeschaltet\n");
             break;
    case 4 : printf("Kaffee wird gemacht\n");
             break;
    case 5 : printf("Gute Nacht\n");
             break;
    default: printf("Falsche Eingabe !\n");
  }
  return 0;
}
