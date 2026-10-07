// kap007/listing005.c
#include <stdio.h>

int intcmp(unsigned int val1, unsigned int val2) {
  if( val1 > val2 ) {
    return val1;
  }
  else if (val1 < val2) {
    return val2;
  }
  return 0; // Beide sind gleich.
}

int main(void) {
  unsigned int ival1 = 0, ival2 = 0;
  printf("Bitte zwei Ganzzahlen eingeben: ");
  if( scanf("%u %u", &ival1, &ival2) != 2 ) {
    printf("Fehler beider Eingabe...\n");
    return 1;
  }
  int cmp = intcmp( ival1, ival2 );
  if(cmp != 0) {
    printf("%d ist der hoehere Wert\n", cmp);
  }
  else {
    printf("Beide Werte sind gleich\n");
  }
  return 0;
}
