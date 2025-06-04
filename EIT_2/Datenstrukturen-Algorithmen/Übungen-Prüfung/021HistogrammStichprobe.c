#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#define MaxWerte 100

int main() {
    int N = MaxWerte;
    int Proben[10] = { 0 };

    scanf("%d", &N);

    for (int i = 0; i < N; i++) {
        int j;
        scanf("%d", &j);
        Proben[j]++;
    }

    for (int i = 0; i < N; i++) {
        printf("%d:", i);
        for (int j = 0; j < Proben[i]; j++) {
            printf("*");
        }
        printf("\n");
    }

    return 0;
}