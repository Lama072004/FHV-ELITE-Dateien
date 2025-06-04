#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>



int main() {
    char sum[101];  // max 100 Zeichen + \0
    scanf("%s", sum);

    int zahlen[100];  // maximal 100 Ziffern
    int count = 0;

    // Ziffern auslesen
    for (int i = 0; sum[i] != '\0'; i++) {
        if (sum[i] >= '0' && sum[i] <= '9') {
            zahlen[count++] = sum[i] - '0';
        }
    }

    // Sortieren (z.?B. Bubble Sort, weil count ? 100)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (zahlen[j] > zahlen[j + 1]) {
                int tmp = zahlen[j];
                zahlen[j] = zahlen[j + 1];
                zahlen[j + 1] = tmp;
            }
        }
    }

    // Ausgabe mit + zwischen den Zahlen
    for (int i = 0; i < count; i++) {
        printf("%d", zahlen[i]);
        if (i < count - 1)
            printf("+");
    }

    printf("\n");
    return 0;
}
