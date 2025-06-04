#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#define MAXLAENGE 400

int main() {
    char text[MAXLAENGE];
    char TextGrid[20][20];
    int count = 0;

    // Eingabe
    fgets(text, MAXLAENGE, stdin);

    // Zeichen zählen
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            count++;
        }
    }

    int size = sqrt(count);

    // Grid mit Buchstaben füllen
    int index = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            TextGrid[index / size][index % size] = text[i];
            index++;
        }
    }

    /* Ausgabe
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            printf("%c ", TextGrid[y][x]);
        }
        printf("\n");
    }
    */

    for (int k = 0; k < size; k++) {
        for (int l = 0; l < size; l++) {
            printf("%c", TextGrid[l][k]);
        }
    }
    return 0;
}
