// kap0009/listing001.c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int iArray[3];
  // Array mit Werten initialisieren
  iArray[0] = 1234;
  iArray[1] = 3456;
  iArray[2] = 7890;

  // Inhalt der Array-Elemente ausgeben
  printf("iArray[0] = %d\n", iArray[0]);
  printf("iArray[1] = %d\n", iArray[1]);
  printf("iArray[2] = %d\n", iArray[2]);
  return EXIT_SUCCESS;
}
