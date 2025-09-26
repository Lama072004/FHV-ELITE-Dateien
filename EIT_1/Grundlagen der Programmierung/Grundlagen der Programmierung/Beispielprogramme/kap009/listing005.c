// kap009/listing005.c
#include <stdio.h>
#include <stdlib.h>
#define MAX 3

void readArray( int arr[], int n ) {
  for(int i=0; i < n; i++) {
    printf("[%d] = %d\n", i, arr[i]);
  }
}

void initArray( int arr[], int n) {
  for(int i=0; i < n; i++) {
    arr[i] = i+i;
  }
}

int main(void) {
  int iArr[MAX];
  initArray( iArr, MAX );
  readArray( &iArr[0], MAX );
  return EXIT_SUCCESS;
}
