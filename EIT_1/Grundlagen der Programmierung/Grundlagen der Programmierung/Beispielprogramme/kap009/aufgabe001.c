// kap009/aufgabe001.c
#include <stdio.h>
#include <stdlib.h>
#define MAX 10

int main(void) {
  int ival[MAX];
  for(int i = MAX; i > 0; i--) {
    ival[i] = i;
  }
  for(int i = 0; i < MAX; i++) {
    printf("%d\n", ival[i]);
  }
  return EXIT_SUCCESS;
}
