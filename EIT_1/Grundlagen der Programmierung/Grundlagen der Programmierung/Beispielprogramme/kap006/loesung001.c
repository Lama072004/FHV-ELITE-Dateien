// kap006/loesung001.c
#include <stdio.h>

int main(void) {
  double geld=0.0, zs_tmp=0.0, zs=0.0;
  unsigned int jahre=0;
  printf("Welchen Geldbetrag wollen Sie einzahlen: ");
  if( scanf("%lf", &geld) != 1 ) {
    printf("Fehler bei der Eingabe...\n");
    return 1;
  }
  printf("Wie viel Zinsen bekommen Sie hierbei(%%): ");
  if( scanf("%lf", &zs_tmp) != 1 ) {
    printf("Fehler bei der Eingabe...\n");
    return 1;
  }
  zs = (zs_tmp / 100) + 1.0f;
  printf("Wie viele Jahre wollen Sie sparen      : ");
  if( scanf("%u", &jahre) != 1 ) {
    printf("Fehler bei der Eingabe...\n");
    return 1;
  }
  for(unsigned int i = 0; i < jahre; geld=geld*zs, i++ ) {
    printf("%2u. Jahr: %.2lf\n",i+1, geld);
  }
  return 0;
}
