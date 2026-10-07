// kap008/loesung003.c
#include <stdio.h>
#include <stdlib.h>
#define DEBUG_TIME printf("%s / %s\n", __DATE__,__TIME__);
#define DEBUG_LINE printf("%d / %s\n", __LINE__,__FILE__);
#define DEBUG_ALL { DEBUG_TIME DEBUG_LINE }

int main(void) {
  DEBUG_ALL;
  return EXIT_SUCCESS;
}
