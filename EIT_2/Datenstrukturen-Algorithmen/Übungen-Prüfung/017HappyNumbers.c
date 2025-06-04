#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

int zifQuadSum(int n) {
    int quadSum = 0;
    int digit = 0;
    while (n > 0) {
        digit = n % 10;
        quadSum += digit * digit;
        n /= 10;
    }
    return quadSum;
}

int main() {
    int Input=0;
    int count = 0;

    scanf("%d", &Input);

    while (Input != 1 && count <1000 ) {
        Input = zifQuadSum(Input);
        count+=1;
    }
    
    if (Input == 1)
        printf("True");
    else printf("False");

    return 0;
}