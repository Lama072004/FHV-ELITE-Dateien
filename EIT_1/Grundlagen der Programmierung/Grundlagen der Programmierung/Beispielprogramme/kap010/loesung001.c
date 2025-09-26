// kap010/loesung001.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 255

char *int2string(int number, int n) {
  static char buf[MAX];
  int i;
  if( n > MAX ) {
    return ("Fehler n > MAX");
  }
  for( i=0; i < n; i++ ) {
    buf[n-i-1] = (number % 10) + 48;
    number = number / 10;
  }
  buf[n] = '\0';
  return buf;
}

int main(void) {
  int val1 = 1234, val2 = 456789;
  char *ptr=NULL;
  ptr = int2string(val1, 4);
  printf("%s\n", ptr);
  ptr = int2string(val2, 9);
  printf("%s\n", ptr);
  return EXIT_SUCCESS;
}
