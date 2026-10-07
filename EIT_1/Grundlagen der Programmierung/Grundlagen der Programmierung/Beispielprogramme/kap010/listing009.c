// kap010/listing009.c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int (*fptr)(const char*, ...);
  fptr = printf;
  (*fptr)("Hallo Welt mit Funktionszeigern\n");
  return EXIT_SUCCESS;
}
