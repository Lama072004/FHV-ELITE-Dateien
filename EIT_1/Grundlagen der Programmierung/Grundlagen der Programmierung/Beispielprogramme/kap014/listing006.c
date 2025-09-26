// kap014/listing006.c
#include <stdio.h>
#include <stdlib.h>

int main (void) {
  char text[]="Tiefstand";
  int breite = 20;
  printf("|01234567890123456789|\n");
  printf("|%s|\n",text);
  printf("|%20s|\n",text);
  printf("|%-20s|\n",text);
  printf("|%20s|\n",text+4);
  printf("|%-20s|\n",text+4);
  printf("|%*s|\n",breite, text);
  return EXIT_SUCCESS;
}
