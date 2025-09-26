// kap005/listing003.c
#include <stdio.h>

int main(void) {
  int val1 = 0, val2 = 0;
  printf("Bitte zwei Ganzzahlwerte eingeben: ");
  int check = scanf("%d %d", &val1, &val2);
  if(check != 2) {
    printf("Fehler bei der Eingabe...\n");
    return 1;
  }
  const int max = (val1 > val2) ?val1 :val2;
  printf("Hoeherer Wert: %d\n", max);
  return 0;
}
