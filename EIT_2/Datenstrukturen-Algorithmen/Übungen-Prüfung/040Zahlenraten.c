#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
    int randomNumber = (rand() % 100) + 1;
    int Input = -1;
    while (Input != randomNumber) {
        scanf("%d", &Input);
        if (Input > randomNumber)
            printf("Zu hoch!\n");
        else if(Input< randomNumber)
            printf("Zu niedrig!\n");
        else printf("Herzlichen Glueckwunsch! Sie haben die Zahl erraten.\n");
    }
    
    return 0;
}