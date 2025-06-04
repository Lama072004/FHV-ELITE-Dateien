#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXSTRING 10
#define MAXTEXT 256

// Hilfsfunktion: macht einen String klein geschrieben
void to_lower(char* str) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower((unsigned char)str[i]);
}

// Hilfsfunktion: prüft, ob `substr` in `word` vorkommt (case-insensitive)
bool contains_substring(char* word, char* substr) {
    char temp_word[MAXTEXT];
    char temp_sub[MAXSTRING];
    strcpy(temp_word, word);
    strcpy(temp_sub, substr);
    to_lower(temp_word);
    to_lower(temp_sub);
    return strstr(temp_word, temp_sub) != NULL;
}

int main() {
    char string[MAXSTRING];
    char text[MAXTEXT];
    int count = 0;

    // Eingabe einlesen
    fgets(string, MAXSTRING, stdin);
    fgets(text, MAXTEXT, stdin);

    // '\n' entfernen
    string[strcspn(string, "\n")] = '\0';
    text[strcspn(text, "\n")] = '\0';

    // Text in Wörter aufteilen
    char* word = strtok(text, " ");
    while (word != NULL) {
        if (contains_substring(word, string)) {
            count++;
        }
        word = strtok(NULL, " ");
    }

    printf("%d\n", count);
    return 0;
}