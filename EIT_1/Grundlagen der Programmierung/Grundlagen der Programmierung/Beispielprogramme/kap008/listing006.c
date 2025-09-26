// kap008/listing006.c
#include <stdio.h>
#include <stdlib.h>

#if __STDC_VERSION__ >= 199901L
void eineFunktion(void) {
  printf("Name der Funktion: %s\n", __func__);
}
#else
void eineFunktion(void) {
  printf("Compiler kann kein C99\n");
}
#endif

int main(void) {
#if __STDC_VERSION__ >= 201112L
  printf("Die Ausrichtung von char: %zd\n",_Alignof(char));
#else
  printf("Compiler kann kein C11\n");
#endif

  printf("Datum der Uebersetzung : %s\n", __DATE__);
  printf("Zeit der Uebersetzung  : %s\n", __TIME__);
  printf("Zeile: %3d | Datei: %s\n", __LINE__, __FILE__);
  eineFunktion();
  return EXIT_SUCCESS;
}
