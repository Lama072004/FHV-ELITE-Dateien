#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main()
{
    int W, H;
    scanf("%d%d", &W, &H);

    int food[10][10];
    int dp[10][10];  // Maximale Futtermenge bis zu jeder Zelle

    // Eingabe lesen
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            scanf("%d", &food[i][j]);
        }
    }

    // DP initialisieren und berechnen
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            int from_top = (i > 0) ? dp[i - 1][j] : 0;
            int from_left = (j > 0) ? dp[i][j - 1] : 0;
            dp[i][j] = food[i][j] + ((from_top > from_left) ? from_top : from_left);
        }
    }

    printf("%d\n", dp[H - 1][W - 1]);

    return 0;
}
