#include <stdio.h>
#include <stdlib.h>

int main (void) {
  int val = 123456789;
  printf("|012345678901|\n");
  printf("|%d|\n",val);
  printf("|%+12d|\n",val);
  printf("|%-12d|\n",val);
  printf("|%012d|\n",val);
  printf("|%-#12o|\n",val);
  printf("|%#x|\n", val);
  printf("|%#012X|\n", val);
  return EXIT_SUCCESS;
}
