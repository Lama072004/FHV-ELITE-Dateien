#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXANZAHL 10

int main() {
    int n = MAXANZAHL;
    char Zeichen[MAXANZAHL];
    int Ziffern[MAXANZAHL];

    scanf("%d\n", &n);
    fgets(Zeichen, n+1, stdin);
    for (int i = 0; i < n; i++) {
        scanf("%d", &Ziffern[i]);
    }

    for (int j = 0; j < n; j++) {
        printf("%c", Zeichen[Ziffern[j]-1]);
    }
    

    return 0;
}