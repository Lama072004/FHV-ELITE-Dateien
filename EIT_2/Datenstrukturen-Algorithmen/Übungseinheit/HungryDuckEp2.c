#include <stdio.h>
#include <stdlib.h>

#define MAX_W 100

int main() {
    int W, H;
    scanf("%d%d", &W, &H);

    int dp[MAX_W] = {0};

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            int food;
            scanf("%d", &food);

            if (i == 0 && j == 0) {
                dp[j] = food;
            } else if (i == 0) {
                dp[j] = dp[j - 1] + food;
            } else if (j == 0) {
                dp[j] = dp[j] + food;
            } else {
                int max = dp[j] > dp[j - 1] ? dp[j] : dp[j - 1];
                dp[j] = max + food;
            }
        }
    }

    printf("%d\n", dp[W - 1]);
    return 0;
}
