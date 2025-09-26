// kap007/listing012.c
#include <stdio.h>

void counter(void) {
  static int ival;
  printf("ival: %d\n", ival);
  ++ival;
}

int main(void) {
  counter();
  counter();
  counter();
  return 0;
}
