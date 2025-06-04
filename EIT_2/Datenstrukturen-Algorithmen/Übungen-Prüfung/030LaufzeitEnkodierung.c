#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#define MAXGITTER 7

int main() {
    int Gittergr��e = MAXGITTER;
    char Gitter[MAXGITTER][MAXGITTER];
    scanf("%d\n", &Gittergr��e);

    // Einlesen des Gitters
    for (int i = 0; i < Gittergr��e; i++) {
        for (int j = 0; j < Gittergr��e; j++) {
            scanf("%c", &Gitter[i][j]);
        }
        getchar(); // Zeilenumbruch nach jeder Zeile �berspringen
    }

    // Run-Length Encoding
    char current = Gitter[0][0];
    int count = 1;

    for (int i = 0; i < Gittergr��e; i++) {
        for (int j = 0; j < Gittergr��e; j++) {
            // �berspringe erstes Zeichen (bereits gesetzt)
            if (i == 0 && j == 0) continue;

            char zeichen = Gitter[i][j];

            if (zeichen == current) {
                count++;
            }
            else {
                if (count == 1)
                    printf("%c", current);
                else
                    printf("%d%c", count, current);

                current = zeichen;
                count = 1;
            }
        }
    }

    // Letzte Gruppe ausgeben
    if (count == 1)
        printf("%c", current);
    else
        printf("%d%c", count, current);

    return 0;
}
