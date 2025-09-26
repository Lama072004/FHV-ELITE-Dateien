// kap004/listing002.c
#include <stdio.h>

int main(void) {
  int iVar1 = 0, iVar2 = 0;
  printf("Bitte zwei Ganzzahlen eingeben: ");
  int check = scanf("%d %d", &iVar1, &iVar2);
  if( check != 2 ) {
    printf("Fehler: Zwei Ganzzahlen erwartet!\n");
    return 1;  // Programm beenden
  }
  printf("%d Wert(e) eingelesen : ", check);
  printf("%d und %d\n", iVar1, iVar2);
  return 0;
}
