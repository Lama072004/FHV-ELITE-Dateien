#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main() {
    int W, H;
    scanf("%d%d", &W, &H);
    int N;
    scanf("%d", &N);
    int X0, Y0;
    scanf("%d%d", &X0, &Y0);

    int minX = 0, maxX = W - 1;
    int minY = 0, maxY = H - 1;

    while (1) {
        char PosBomb[4];
        scanf("%s", PosBomb); // z.B. "UR", "D", etc.

        if (strcmp(PosBomb, "U") == 0) {
            maxY = Y0 - 1;
        } else if (strcmp(PosBomb, "UR") == 0) {
            maxY = Y0 - 1;
            minX = X0 + 1;
        } else if (strcmp(PosBomb, "R") == 0) {
            minX = X0 + 1;
        } else if (strcmp(PosBomb, "DR") == 0) {
            minY = Y0 + 1;
            minX = X0 + 1;
        } else if (strcmp(PosBomb, "D") == 0) {
            minY = Y0 + 1;
        } else if (strcmp(PosBomb, "DL") == 0) {
            minY = Y0 + 1;
            maxX = X0 - 1;
        } else if (strcmp(PosBomb, "L") == 0) {
            maxX = X0 - 1;
        } else if (strcmp(PosBomb, "UL") == 0) {
            maxY = Y0 - 1;
            maxX = X0 - 1;
        }

        // Nächster Sprung: Mittelpunkt des Bereichs
        X0 = (minX + maxX) / 2;
        Y0 = (minY + maxY) / 2;

        printf("%d %d\n", X0, Y0);
    }

    return 0;
}
