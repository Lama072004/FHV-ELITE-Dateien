// kap009/listing007.c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char string1[20] = "String";
  char string2[20] = {'S', 't', 'r', 'i', 'n', 'g', '\0'};
  printf("%s\n", string1);
  printf("%s\n", string2);
  return EXIT_SUCCESS;
}
