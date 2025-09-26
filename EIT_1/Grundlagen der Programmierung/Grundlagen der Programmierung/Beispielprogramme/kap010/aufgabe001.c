// kap010/aufgabe001.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 255

char *mysearch( char *str, char ch ) {
  char buf[MAX] = "";
  int i = 0;
  while( str[i] != '\0' ) {
    if( str[i] == ch ) {
      strncpy(buf, &str[i], MAX);
      return buf;
    }
    i++;
  }
  return buf;
}

int main(void) {
  char *str = mysearch("Hallo Welt", 'W');
  printf("%s\n", str);
  return EXIT_SUCCESS;
}
