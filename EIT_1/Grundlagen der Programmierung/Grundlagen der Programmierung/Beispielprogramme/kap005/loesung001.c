// kap005/loesung001.c
#include <stdio.h>

int main(void) {
  int ival = 0;
  printf("Zahl zwischen 1-100 eingeben: ");
  if( scanf("%d", &ival) != 1 ) {
    printf("Fehler bei der Eingabe...\n");
    return 1;
  }
  if( (ival >= 1) && (ival <= 100) ) {
    if( ival % 2 ) {
      printf("Die Zahl ist ungerade\n");
    }
    else {
      printf("Die Zahl ist gerade\n");
    }
  }
  else {
    printf("Die Zahl war nicht 1-100!\n");
  }
  return 0;
}
