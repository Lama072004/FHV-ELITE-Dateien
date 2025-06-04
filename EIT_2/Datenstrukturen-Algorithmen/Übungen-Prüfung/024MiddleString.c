#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STRING_LENGTH 1048
#define MAX_WORDS 512

int main() {
    char text[MAX_STRING_LENGTH];
    char* words[MAX_WORDS];
    int count = 0;

    // Eingabe einlesen
    fgets(text, MAX_STRING_LENGTH, stdin);

    // Zerlege den Text in Wörter
    char* token = strtok(text, " ");
    while (token != NULL) {
        words[count++] = token;
        token = strtok(NULL, " ");
    }

    // Mittleres Wort oder Kombination der zwei mittleren
    if (count % 2 == 1) {
        // ungerade Anzahl
        printf("%s\n", words[count / 2]);
    }
    else {
        // gerade Anzahl
        printf("%s%s\n", words[count / 2 - 1], words[count / 2]);
    }

    return 0;
}