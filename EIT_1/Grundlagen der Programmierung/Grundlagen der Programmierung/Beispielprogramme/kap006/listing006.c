// kap006/listing006.c
#include <stdio.h>

int main(void) {
  int sum=0;
  for(int ival=0; ival < 20; ival++) {
    if(ival % 2) {
      continue;
    }
    sum+=ival;
  }
  printf("Summe gerader Zahlen: %d\n", sum);
  return 0;
}
