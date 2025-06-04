#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXZAHL 100

int main() {
    int Zahl = MAXZAHL;
    scanf("%d", &Zahl);

    while (Zahl != 1) {
        if (Zahl % 2 == 0)
            Zahl /= 2;
        else
            Zahl = (Zahl * 3 + 1);
        printf("%d ", Zahl);
    }

    return 0;
}
