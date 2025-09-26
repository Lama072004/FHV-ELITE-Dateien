// kap004/listing004.c
#include <stdio.h>

int main(void) {
  int ival = 1;
  printf("ival : %d\n", ival);
  ival++;
  printf("ival : %d\n", ival);
  printf("ival : %d\n", ival++);
  printf("ival : %d\n", ival);
  printf("ival : %d\n", ++ival);
  return 0;
}
