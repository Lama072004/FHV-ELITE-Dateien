// kap012/listing006.c
#include <stdio.h>
#include <stdlib.h>

typedef struct rgb {
  unsigned short red, green, blue;
} RGB;

typedef union color {
  RGB rgb;
  unsigned int hexRGB;
} Color;

int main(void) {
  Color color01, color02;
  color01.rgb.red   = 127;
  color01.rgb.green = 255;
  color01.rgb.blue  = 255;
  color02.hexRGB = 0x7fffff;

  printf("color01: %x%x%x\n",
    color01.rgb.red,color01.rgb.green,color01.rgb.blue);
  printf("color02: %x\n", color02.hexRGB);
  return EXIT_SUCCESS;
}
