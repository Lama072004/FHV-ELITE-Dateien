#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
    char amountStr[10];
    fgets(amountStr, sizeof(amountStr), stdin);

    // Betrag in float, beginnend bei Index 1 (überspringt '$')
    float amount = atof(&amountStr[1]);

    // Betrag in Cent umwandeln (z.?B. 4.58 ? 458)
    int cents = (amount * 100 + 0.5);  // Rundet korrekt

    int dollars = cents / 100;
    cents %= 100;

    int quarters = cents / 25;
    cents %= 25;

    int dimes = cents / 10;
    cents %= 10;

    int nickels = cents / 5;
    cents %= 5;

    int pennies = cents;

    printf("%d %d %d %d %d\n", dollars, quarters, dimes, nickels, pennies);

    return 0;
}