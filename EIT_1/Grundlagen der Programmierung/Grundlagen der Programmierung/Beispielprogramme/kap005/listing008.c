// kap005/listing008.c
#include <stdio.h>

int main(void) {
  int geheimnummer = 0;
  printf("Geheimnummer eingeben: ");
  int check = scanf("%d", &geheimnummer);
  if( check != 1 ) {
    printf("Fehler bei der Eingabe\n");
    return 1;
  }
  else if( ! (geheimnummer == 123456) ) {
    printf("Geheimnummer ist falsch!\n");
  }
  else {
    printf("Geheimnummer ist richtig!\n");
  }
  return 0;
}
