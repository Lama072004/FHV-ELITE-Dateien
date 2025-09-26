// kap009/listing004.c
#include <stdio.h>
#include <stdlib.h>
#define MAX 3

int main(void) {
  double dval[MAX];
  for(int i = 0; i < MAX; i++) {
    printf("%d. double-Wert : ", i+1);
    if (scanf("%lf", &dval[i]) != 1) {
      printf("Fehler bei der Eingabe...\n");
      return EXIT_FAILURE;
    }
  }
  printf("Sie gaben ein : ");
  for(int i = 0; i < MAX; i++) {
    printf("%.2lf ", dval[i]);
  }
  printf("\n");
  return EXIT_SUCCESS;
}
