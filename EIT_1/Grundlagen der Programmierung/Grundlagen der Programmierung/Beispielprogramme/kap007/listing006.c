// kap007/listing006.c
#include <stdio.h>

inline double kreisflaeche(double r) {
  return  3.14159265358979323846 * r * r;
}

int main(void) {
  double radius = 1.0, flaeche = 0.0;
  for(int i = 0; i < 100; i++) {
    flaeche = kreisflaeche(radius);
    printf("Radius: %lf Flaeche: %lf\n", radius, flaeche);
    radius += 1.0;
  }
  return 0;
}
