#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 201

void to_lowercase(char* str) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower(str[i]);
}

int main() {
    char s1[101], s2[101];
    char s1s1[MAX];

    fgets(s1, sizeof(s1), stdin);
    fgets(s2, sizeof(s2), stdin);

    // Newline entfernen
    s1[strcspn(s1, "\n")] = '\0';
    s2[strcspn(s2, "\n")] = '\0';

    // Beide in Kleinbuchstaben
    to_lowercase(s1);
    to_lowercase(s2);

    if (strlen(s1) != strlen(s2)) {
        printf("false\n");
        return 0;
    }

    // s1s1 = s1 + s1
    strcpy(s1s1, s1);
    strcat(s1s1, s1);

    if (strstr(s1s1, s2)) {
        printf("true\n");
    }
    else {
        printf("false\n");
    }

    return 0;
}