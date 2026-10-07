#include <stdio.h>
#include <math.h>

#define MAX_VAULTS 20
#define MAX_ROBBERS 5

int main() {
    int R, V;
    scanf("%d", &R);
    scanf("%d", &V);

    long long vault_times[MAX_VAULTS];
    for (int i = 0; i < V; ++i) {
        int C, N;
        scanf("%d %d", &C, &N);
        int vowels = C - N;

        long long combinations = pow(10, N) * pow(5, vowels);
        vault_times[i] = combinations;
    }

    long long robber_time[MAX_ROBBERS] = {0};

    for (int i = 0; i < V; ++i) {
        // Find the robber who is free the earliest
        int min_index = 0;
        for (int j = 1; j < R; ++j) {
            if (robber_time[j] < robber_time[min_index]) {
                min_index = j;
            }
        }
        // Assign vault i to that robber
        robber_time[min_index] += vault_times[i];
    }

    // Find the max time any robber has spent (heist ends then)
    long long max_time = 0;
    for (int i = 0; i < R; ++i) {
        if (robber_time[i] > max_time) {
            max_time = robber_time[i];
        }
    }

    printf("%lld\n", max_time);
    return 0;
}
