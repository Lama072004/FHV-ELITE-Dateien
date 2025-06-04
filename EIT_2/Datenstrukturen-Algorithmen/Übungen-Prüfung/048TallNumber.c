#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 10000000

int main() {
    int n = MAX;
    int letzteZiffer;
    int ZifferCount = 0;
    int count = 0;
    scanf("%d", &n);

    while (n > 0) {
        letzteZiffer = (n % 10);
        n /= 10;
        ZifferCount++;
        if (letzteZiffer >= n % 10)
            count++;
    }

    if (count == ZifferCount)
        printf("true");
    else printf("false");

    return 0;
}