// kap003/listing007.c
#include <stdio.h>
#include <stdint.h>

int main(void) {
  int64_t bigVar = 12345678;
  printf("bigVar          : %lld\n", bigVar);
  printf("sizeof(int64_t) : %zu\n", sizeof(int64_t));
  printf("INT64_MAX       : %lld\n", INT64_MAX);
  return 0;
}
