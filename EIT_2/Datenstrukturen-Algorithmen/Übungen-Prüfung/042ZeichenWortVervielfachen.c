#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXANZAHL 100

int main() {
    char Wort[MAXANZAHL];
    int Buchstaben[26] = { 0 }; // Zählt a-z

    fgets(Wort, MAXANZAHL, stdin);

    for (int i = 0; Wort[i] != '\0' && Wort[i] != '\n'; i++) {
        char ch = Wort[i];
        int index = ch - 'a'; // a=0, b=1, ..., z=25

        Buchstaben[index]++; // Wie oft ist der Buchstabe bisher erschienen?

        // Wiederhole das Zeichen entsprechend der aktuellen Häufigkeit
        for (int j = 0; j < Buchstaben[index]; j++) {
            printf("%c", ch);
        }
    }

    printf("\n");
    return 0;
}