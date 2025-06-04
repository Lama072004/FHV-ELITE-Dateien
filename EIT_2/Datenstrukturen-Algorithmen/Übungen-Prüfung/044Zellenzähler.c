#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 100

int main() {
    int W, H;
    int X, Y;
    char gitter[MAX][MAX];

    // Eingabe: Breite und Höhe
    scanf("%d %d", &W, &H);

    // Eingabe: Zielkoordinaten
    scanf("%d %d", &X, &Y);

    // Eingabe: Gitterzeilen einlesen
    for (int i = 0; i < H; i++) {
        scanf("%s", gitter[i]);
    }

    int nachbarn = 0;

    // Alle 8 Richtungen überprüfen
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue; // sich selbst überspringen

            int nx = X + dx;
            int ny = Y + dy;

            // Grenzen prüfen
            if (nx >= 0 && nx < W && ny >= 0 && ny < H) {
                if (gitter[ny][nx] == '1') {
                    nachbarn++;
                }
            }
        }
    }

    printf("%d\n", nachbarn);
    return 0;
}