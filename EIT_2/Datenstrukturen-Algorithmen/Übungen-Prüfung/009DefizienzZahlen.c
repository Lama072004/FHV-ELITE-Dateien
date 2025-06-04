#include <stdio.h>

int SumTeiler(int n) {
    int summe = 0;
    for (int i = 1; i <= n; i++) {
        if (n % i == 0)
            summe += i;
    }
    return summe;
}

int main() {
    int N = 0;
    scanf_s("%d", &N);  // scanf reicht aus; scanf_s ist nicht überall verfügbar

    for (int i = 0; i < N; i++) {
        int A, B;
        scanf_s("%d %d", &A, &B);

        int Defizienz = 0;  // Zurücksetzen für jeden Bereich

        for (int j = A; j <= B; j++) {
            int teiler_summe = SumTeiler(j);
            int doppelt = 2 * j;

            if (teiler_summe < doppelt) {
                Defizienz += (doppelt - teiler_summe);
            }
        }
        printf("%d\n", Defizienz);
    }

    return 0;
}
