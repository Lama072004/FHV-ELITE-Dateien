// kap010/loesung003.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 2048

int main(void) {
  char buf[MAX]="";
  const char token[] = " ,.:!?\t\n";
  char *worte[MAX];

  printf("Bitte den Text eingeben: ");
  if( fgets(buf, MAX, stdin) == NULL ) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  char* ptr = strtok(buf, token);
  size_t i = 0;
  while(ptr != NULL) {
    worte[i] = ptr;
    ptr = strtok(NULL, token);
    i++;
  }
  printf("Zerlegt in einzelne Woerter:\n");
  for(size_t j=0; j < i; j++) {
    printf("%s\n", worte[j]);
  }
  return EXIT_SUCCESS;
}
