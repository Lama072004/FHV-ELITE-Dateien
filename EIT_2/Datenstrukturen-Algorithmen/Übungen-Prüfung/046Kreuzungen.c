#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 100

int main() {
    char Grid[MAX][MAX];
    int width, height;
    int count = 0;

    scanf("%d %d", &width, &height);

    for (int i = 0; i < height; i++) {
        scanf("%s", Grid[i]);
    }

    /*
    for (int i = 0; i < height; i++) {
        printf("%s\n", Grid[i]);
    }
    */
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            if (Grid[y][x] == 'X' &&
                Grid[y - 1][x] == 'X' &&
                Grid[y + 1][x] == 'X' &&
                Grid[y][x - 1] == 'X' &&
                Grid[y][x + 1] == 'X') {
                count++;
            }
        }
    }

    printf("%d\n", count);
    return 0;
}