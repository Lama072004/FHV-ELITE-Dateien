// kap003/listing004.c
#include <stdio.h>

int main(void) {
  int ival = 0;
  double dval = 0;
  printf("sizeof(ival)  : %zu\n", sizeof(ival));
  printf("sizeof(dval)  : %zu\n", sizeof(dval));
  // So geht es auch
  printf("sizeof(float) : %zu\n", sizeof(float));
  size_t sz = sizeof(char);
  printf("sizeof(char)  : %zu\n", sz);
  return 0;
}