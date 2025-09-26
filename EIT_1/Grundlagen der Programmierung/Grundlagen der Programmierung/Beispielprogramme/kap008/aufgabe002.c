// kap008/aufgabe002.c
#include <stdio.h>
#include <stdlib.h>
#define CNT 10

int main(void) {
  int i;
#undef CNT
#define CNT 5
  for( i = 0; i < CNT; i++) {
#undef CNT
#define CNT 20
     printf("%d\n", i);
  }
  return EXIT_SUCCESS;
}
