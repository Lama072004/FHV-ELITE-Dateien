#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
    char input[1000];
    char* words[200];  // maximal 200 Wörter
    int wordCount = 0;

    // Eingabe lesen
    fgets(input, sizeof(input), stdin);

    // Zeilenumbruch entfernen
    input[strcspn(input, "\n")] = 0;

    // Wörter trennen
    char* token = strtok(input, " ");
    while (token != NULL) {
        words[wordCount++] = token;
        token = strtok(NULL, " ");
    }

    // Wörter rückwärts ausgeben
    for (int i = wordCount - 1; i >= 0; i--) {
        printf("%s", words[i]);
        if (i > 0) {
            printf(" ");
        }
    }

    printf("\n");
    return 0;
}