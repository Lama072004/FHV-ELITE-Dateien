// kap009/listing002.c
#include <stdio.h>
#include <stdlib.h>
#define MAX 10

int main(void) {
  unsigned int iArray[MAX];
  // Werte an alle Elemente
  for(unsigned int i = 0; i < MAX; i++) {
    iArray[i]=i*i;
  }
  // Werte ausgeben
  for(unsigned int i = 0; i < MAX; i++) {
    printf("iArray[%d] = %d\n", i, iArray[i]);
  }
  return EXIT_SUCCESS;
}
