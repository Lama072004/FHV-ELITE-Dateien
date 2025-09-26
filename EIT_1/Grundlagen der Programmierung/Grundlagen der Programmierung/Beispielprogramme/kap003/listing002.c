// kap003/listing002.c
#include <stdio.h>

int main(void) {
  char ch01 = 'A';
  char ch02 = 66;  // besser wäre 'B'
  printf("Dezimal: %d; Zeichen: %c\n", ch01, ch01);
  printf("Dezimal: %d; Zeichen: %c\n", ch02, ch02);
  return 0;
}
