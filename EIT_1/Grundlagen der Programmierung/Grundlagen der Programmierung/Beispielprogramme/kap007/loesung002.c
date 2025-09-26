// kap007/loesung002.c
#include <stdio.h>
#include <stdlib.h>

float volumen_Rect(float l, float b, float h) {
  float volumen = l*b*h;
  return volumen;
  // oder gleich: return l*b*h;
}

int main(void) {
  float vol = volumen_Rect(10.0, 10.0, 12.5);
  printf("Volumen: %f\n", vol);
  return EXIT_SUCCESS;
}
