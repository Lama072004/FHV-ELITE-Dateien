#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>



int main() {
    int gäste;
    int anstossen;

    scanf("%d", &gäste);
    
    anstossen = (gäste * (gäste - 1)) / 2;

    printf("%d", anstossen);
    
    return 0;
}