#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>


#define MAX_TYPES 30

int main() {
    int n;
    scanf("%d", &n);

    char fruitTypes[MAX_TYPES];
    int required[MAX_TYPES];

    for (int i = 0; i < n; ++i) {
        // Leerzeichen vor %c verhindert das Einlesen von Newlines
        scanf(" %c %d", &fruitTypes[i], &required[i]);
    }

    char fruits[1001]; // genug Platz für viele Früchte
    scanf("%s", fruits);

    int count[256] = { 0 }; // ASCII-Zeichen zählen

    for (int i = 0; fruits[i] != '\0'; ++i) {
        count[(unsigned char)fruits[i]]++;
    }

    int totalGlasses = 0;

    for (int i = 0; i < n; ++i) {
        char c = fruitTypes[i];
        int needed = required[i];

        if (needed > 0) {
            totalGlasses += count[(unsigned char)c] / needed;
        }
    }

    printf("%d\n", totalGlasses);
    return 0;
}