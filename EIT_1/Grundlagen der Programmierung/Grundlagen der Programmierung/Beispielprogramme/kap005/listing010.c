// kap005/listing010.c
#include <stdio.h>

int main(void) {
  unsigned int uval1 = 0, uval2 = 0;
  printf("Bitte zwei Ganzzahlen eingeben: ");
  int check = scanf("%u %u", &uval1, &uval2);
  if(check != 2) {
    printf("Fehler bei der Eingabe...\n");
    return 1;
  }
  if( (!uval1) || (!uval2) ) {
    printf("Fehler: Einer der Werte ist gleich 0\n");
  }
  else  {
    printf("%u / %u = %lf\n",
        uval1, uval2, (double)uval1/uval2);
  }
  return 0;
}
