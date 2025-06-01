#include <stdio.h>

int main() {
    int N;
    scanf_s("%d", &N);

    int H = 0;
    int sum = 0;

    // Wir fügen Ebene für Ebene hinzu, solange wir noch genug Blöcke haben
    while (sum + (H + 1) <= N) {
        H++;
        sum += H;
    }

    int R = N - sum;
    printf("%d %d\n", H, R);
    return 0;
}
