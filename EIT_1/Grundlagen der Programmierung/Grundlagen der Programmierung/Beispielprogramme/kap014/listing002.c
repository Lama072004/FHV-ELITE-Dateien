// kap014/listing002.c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  printf("Diesen Text koennen Sie sehen\n");
  if( freopen("logfile.txt", "w", stdout) == NULL ) {
    fprintf(stderr, "Fehler bei freopen\n");
    return EXIT_FAILURE;
  }
  printf("Dieser Text steht in logfile.txt\n");
  return EXIT_SUCCESS;
}
