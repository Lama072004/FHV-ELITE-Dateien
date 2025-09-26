// kap014/loesung002.c
#include <stdio.h>
#include <stdlib.h>
#define CSV "datei.txt"

int main(void) {
  FILE *fp;
  char wochentag[20];
  int einnahmen, ausgaben;
  int ein_ges=0, aus_ges=0;

  fp = fopen(CSV, "r");
  if(fp == NULL) {
    fprintf(stderr, "Fehler beim Oeffnen\n");
    return EXIT_FAILURE;
  }
  printf("%20s\t%5s\t%5s\n", "Tag", "+", "-");
  while( (fscanf(fp, "%[^;];%d;%d\n",
        wochentag, &einnahmen, &ausgaben)) != EOF) {
    printf("%20s\t%5d\t%5d\n", wochentag, einnahmen, ausgaben);
    ein_ges+=einnahmen;
    aus_ges+=ausgaben;
  }
  printf("\n%20s\t%5d\t%5d", "Gesamt", ein_ges, aus_ges);
  printf("\t = %+d\n", ein_ges-aus_ges);
  return EXIT_SUCCESS;
}
