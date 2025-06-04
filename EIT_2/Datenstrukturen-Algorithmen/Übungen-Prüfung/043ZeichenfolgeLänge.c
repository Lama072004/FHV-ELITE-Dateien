#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXANZAHL 150

int main() {
    char C;
    char Wort[MAXANZAHL];

    // Ein Zeichen lesen
    scanf("%c\n", &C);

    // Die Zeichenkette lesen
    fgets(Wort, MAXANZAHL, stdin);

    int i = 0;
    while (Wort[i] != '\0' && Wort[i] != '\n') {
        if (Wort[i] == C) {
            int count = 0;
            int j = i;
            // Zähle, wie viele C hintereinander kommen
            while (Wort[j] == C) {
                count++;
                j++;
            }
            // Anzahl drucken
            printf("%d", count);
            i = j; // weiter nach der Sequenz
        }
        else {
            // Normales Zeichen einfach drucken
            printf("%c", Wort[i]);
            i++;
        }
    }

    printf("\n");
    return 0;
}