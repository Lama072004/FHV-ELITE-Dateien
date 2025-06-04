#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXZAHLEN 10000

int main() {
    int N = MAXZAHLEN;
    int summeGerade = 0;

    scanf("%d", &N);

    for (int i = 2; i <= N; i += 2)
        summeGerade += i;

    printf("%d", summeGerade);

    return 0;
}
