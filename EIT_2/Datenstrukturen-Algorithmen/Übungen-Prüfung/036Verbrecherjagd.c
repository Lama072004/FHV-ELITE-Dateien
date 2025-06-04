#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LEN 256
#define MAX_N 100

int main() {
    char gesuchteZeichen[MAX_LEN];
    int N;

    fgets(gesuchteZeichen, MAX_LEN, stdin);
    gesuchteZeichen[strcspn(gesuchteZeichen, "\n")] = '\0'; // \n entfernen

    scanf("%d\n", &N);

    for (int i = 0; i < N; i++) {
        char zeile[MAX_LEN];
        fgets(zeile, MAX_LEN, stdin);

        int count = 0;
        for (int j = 0; zeile[j] != '\0'; j++) {
            for (int k = 0; gesuchteZeichen[k] != '\0'; k++) {
                if (zeile[j] == gesuchteZeichen[k]) {
                    count++;
                    break;
                }
            }
        }
        printf("%d\n", count);
    }

    return 0;
}