#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

void bubblesort(int array[], int length) {
    int i, j, tmp;

    for (i = 1; i < length; i++) {
        for (j = 0; j < length - i; j++) {
            if (array[j] > array[j + 1]) {
                tmp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = tmp;
            }
        }
    }
}

int main() {
    int N = 0;
    int Zahlen[10];

    scanf("%d", &N);

    for (int i = 0; i < N; i++) {
        scanf("%d", &Zahlen[i]);
    }
    bubblesort(Zahlen, N);

    for (int i = N-1; i >=0; i--) {

        printf("%d", Zahlen[i]);
    }

    return 0;
}