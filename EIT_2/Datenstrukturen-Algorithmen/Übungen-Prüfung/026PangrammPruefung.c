#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#define MAXLAENGE 256

int main() {
    char text[MAXLAENGE];
    bool alphabet['z' - 'a' + 1] = { false };
    int boolCounter = 0;

    // Eingabe
    fgets(text, MAXLAENGE, stdin);

    // In Kleinbuchstaben umwandeln
    for (int i = 0; text[i] != '\0'; i++) {
        text[i] = tolower(text[i]);
    }

    //printf("%s", text);
    
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            alphabet[(text[i] - 'a')] = true;
        }
    }
    
    for (int i = 0; i < sizeof(alphabet); i++) {
        if (alphabet[i] == true)
            boolCounter++;
    }

    if (boolCounter == 26)
        printf("true");
    else printf("false");

    return 0;
}
