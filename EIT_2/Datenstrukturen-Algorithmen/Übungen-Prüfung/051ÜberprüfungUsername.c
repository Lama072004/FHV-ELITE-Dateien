#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXLENGTH 64

int main() {
    char A[MAXLENGTH];
    int ZeichenLaenge = 0;
    int validCount = 0;

    fgets(A, MAXLENGTH, stdin);

    for (int i = 0; A[i] != '\n'; i++)
        ZeichenLaenge++;
    if (ZeichenLaenge > 20 || ZeichenLaenge < 3)
        printf("INVALID");
    else {

        for (int i = 0; i < ZeichenLaenge; i++) {
            if (isalpha(A[i]))
                validCount++;
            else if (isdigit(A[i]))
                validCount++;
        }

        if (validCount == ZeichenLaenge)
            printf("VALID");
        else printf("INVALID");
    }
    

    return 0;
}