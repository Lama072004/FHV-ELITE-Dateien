#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>



int main() {
    int g�ste;
    int anstossen;

    scanf("%d", &g�ste);
    
    anstossen = (g�ste * (g�ste - 1)) / 2;

    printf("%d", anstossen);
    
    return 0;
}