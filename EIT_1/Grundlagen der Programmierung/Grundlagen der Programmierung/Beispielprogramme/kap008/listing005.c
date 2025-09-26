// kap008/listing005.c
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#define TEST

int main(void) {
#if UINT_MAX == 4294967295
  unsigned int val1 = 0;
  unsigned int val2 = 0;
#else
  unsigned long val1 = 0;
  unsigned long val2 = 0;
#endif
  val1 = 500 * 1000;
  val2 = 300 * 200;
  val1+= val2;
  printf("%lu\n", (long)val1);
#ifdef ULLONG_MAX
  unsigned long long uval = val1;
  printf("%llu\n", uval*10000);
#else
  printf("Erweiterte Berechnung nicht moeglich\n");
#endif
#ifdef TEST
  printf("UINT_MAX   : %u\n", UINT_MAX);
  printf("ULONG_MAX  : %lu\n", ULONG_MAX); 
#endif
#if defined(TEST) && defined(ULLONG_MAX)
  printf("ULLONG_MAX : %llu\n", ULLONG_MAX);
#endif  
  return EXIT_SUCCESS;
}
