// kap010/listing008.c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char *german[] = {
   "eins", "zwei", "drei", "vier", "fuenf",
   "sechs", "sieben", "acht", "neun", "zehn"
  };
  char *english[] = {
    "one", "two", "three", "four", "five",
    "six", "seven", "eight", "nine", "ten"
  };
  int ival = 0;
  printf("Bitte eine Ganzzahl von 1 bis 10 : ");
  if( scanf("%d", &ival) != 1 ) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  else if( ival <= 0 || ival > 10 ) {
    printf("Dies ist kein Wert von 1 bis 10\n");
    return EXIT_FAILURE;
  }
  else {
    printf("Ganzzahl: %d\n", ival);
    printf("Englisch: %s\n", english[ival-1]);
    printf("Deutsch : %s\n", german[ival-1]);
  }
  return EXIT_SUCCESS;
}
