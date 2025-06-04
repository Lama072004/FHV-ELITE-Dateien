#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#define MaxN 2147483647

int groesstePotenz(int n, int p) {
    int maxPotenz = 0;
    for (int i = 0; i < sqrt(MaxN); i++) {
        maxPotenz = pow(p, i);

        if (maxPotenz > n)
            return i - 1;
    }
}

int main() {
    unsigned int N = MaxN;
    int P = 0;
    int hoechstePotenz = 0;

    scanf("%d", &N);
    scanf("%d", &P);

    hoechstePotenz = groesstePotenz(N, P);

    printf("%d", hoechstePotenz);
    return 0;
}