// kap005/listing007.c
#include <stdio.h>

int main(void) {
  int opt = 0;
  printf("-1- Option A\n");
  printf("-2- Option B\n");
  printf("-3- Option C\n");
  printf("-4- Option D\n");
  printf("Ihre Auswahl: ");
  int check = scanf("%d", &opt);
  if(check != 1) {
    printf("Fehler bei der Eingabe...\n");
    return 1;
  }
  switch(opt) {
    case 1  : printf("Option A beinhaltet auch ");
    case 2  : printf("Option B\n");
              break;
    case 3  :
    case 4  : printf("Option C und D sind gleich\n");
              break;
    default : printf("Unbekannte Option (%d)?\n" , opt);
  }
  return 0;
}
