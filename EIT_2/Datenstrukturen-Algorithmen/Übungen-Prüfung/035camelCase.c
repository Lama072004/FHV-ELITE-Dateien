#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXLAENGE 256

int main() {
    char eingabe[MAXLAENGE];
    fgets(eingabe, MAXLAENGE, stdin);

    char ausgabe[MAXLAENGE];
    int j = 0;
    bool nachLeerzeichen = false; // Erste Buchstabe soll klein bleiben

    for (int i = 0; eingabe[i] != '\0'; i++) {
        char c = eingabe[i];

        if (c == ' ' || c == '\n') {
            nachLeerzeichen = true;
            continue; // Leerzeichen überspringen
        }

        if (nachLeerzeichen) {
            ausgabe[j++] = toupper(c);
            nachLeerzeichen = false;
        }
        else {
            ausgabe[j++] = tolower(c);
        }
    }

    ausgabe[j] = '\0'; // String abschließen

    printf("%s\n", ausgabe);

    return 0;
}