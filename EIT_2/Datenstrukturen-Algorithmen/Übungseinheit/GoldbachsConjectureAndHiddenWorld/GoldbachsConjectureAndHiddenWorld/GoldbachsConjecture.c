#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX 1000000
bool primes[MAX + 1];
void generatePrimes() {
    for (int i = 2; i <= MAX; i++) {
        primes[i] = true;
    }
    for (int i = 2; i * i <= MAX; i++) {
        for (int j = i * i; j <= MAX; j += i) {
            primes[j] = false;
        }
    }
}

int main() {
    int n;
    scanf("%d", &n);
    generatePrimes();

    for (int i = 0; i < n; i++) {
        int m;

        if (scanf("%d", &m) != 1) {
            return 1;
        }

        int count = 0;

        for (int a = 2; a <= m / 2; a++) {
            int b = m - a;

            if (primes[a] && primes[b]) {
                count++;
            }
        }

        printf("%d\n", count);
    }

    return 0;
}
