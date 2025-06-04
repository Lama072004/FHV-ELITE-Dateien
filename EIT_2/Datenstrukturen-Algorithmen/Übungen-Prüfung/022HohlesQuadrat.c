#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#define MaxWerte 99

int main() {
    int N;
    char Quadrat[MaxWerte][MaxWerte];

    scanf("%d", &N);
    if (N > 99 || N < 0)
        return 0;

    // Alle Felder mit Leerzeichen initialisieren
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            Quadrat[i][j] = ' ';
        }
    }
    // obere und untere Zeile mit '#'
    for (int i = 0; i < N; i++) {
        Quadrat[0][i] = '#';
        Quadrat[N - 1][i] = '#';
    }
    // rechte und linke Zeile mit '#'
    for (int i = 0; i < N; i++) {
        Quadrat[i][0] = '#';
        Quadrat[i][N-1] = '#';
    }
    // Quadrat ausgeben
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%c", Quadrat[i][j]); // Korrekt: %c für Zeichen
        }
        printf("\n");
    }

    return 0;
}