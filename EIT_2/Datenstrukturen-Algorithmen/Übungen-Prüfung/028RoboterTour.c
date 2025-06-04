#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#define MAXGRID 50
#define OFFSET 25  // Damit (0,0) in der Mitte liegt

int main() {
    int Grid[MAXGRID][MAXGRID] = { 0 };
    char route[501];
    int x = OFFSET;
    int y = OFFSET;

    scanf("%s", route);  // Route direkt als String einlesen
    Grid[y][x] = 1;  // Startpunkt besuchen

    for (int i = 0; route[i] != '\0'; i++) {
        char move = route[i];
        if (move == '^') y--;
        else if (move == 'v') y++;
        else if (move == '<') x--;
        else if (move == '>') x++;

        Grid[y][x]++;
    }

    int maxBesuche = 0;
    for (int i = 0; i < MAXGRID; i++) {
        for (int j = 0; j < MAXGRID; j++) {
            if (Grid[i][j] > maxBesuche) {
                maxBesuche = Grid[i][j];
            }
        }
    }

    printf("%d\n", maxBesuche);
    return 0;
}
