#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXLENGTH 20

int main() {
    char A[MAXLENGTH];
    char B[MAXLENGTH];
    int Chiffre[MAXLENGTH];
    int ZeichenLaenge = 0;
    bool isConstant = true;

    fgets(A, MAXLENGTH, stdin);
    fgets(B, MAXLENGTH, stdin);

    for (int i = 0; A[i] != '\n'; i++)
        ZeichenLaenge++;

    for (int j = 0; j < ZeichenLaenge; j++) {
        Chiffre[j] = (A[j] - B[j]);
    }

    for (int j = 1; j < ZeichenLaenge; j++) {
        if (Chiffre[j] != Chiffre[0]) {
            isConstant = false;
            break;
        }
    }

    if (isConstant) {
        printf("true\n");
    }
    else {
        printf("false\n");
    }

    return 0;
}