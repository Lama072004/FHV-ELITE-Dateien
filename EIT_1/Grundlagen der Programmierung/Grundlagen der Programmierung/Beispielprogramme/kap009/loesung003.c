// kap009/loesung003.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 255

void ersetzen( char arr[], int n, char ch1, char ch2 ) {
  for(int  i=0; i < n; i++ ) {
    if( arr[i] == ch1 ) {
      arr[i] = ch2;
    }
  }
}

int main(void) {
  char str[MAX];
  printf("Bitte Mustertext eingeben: ");
  if( fgets( str, MAX, stdin ) == NULL ) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_SUCCESS;
  }
  // 'a' gegen 'x' ersetzen
  ersetzen( str, strlen(str), 'a', 'x');
  printf("%s\n", str);
  return EXIT_SUCCESS;
}
