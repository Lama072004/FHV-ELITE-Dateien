#include <stdio.h>

int main() {
    int N;
    scanf_s("%d", &N);

    int H = 0;
    int sum = 0;

    // Wir f�gen Ebene f�r Ebene hinzu, solange wir noch genug Bl�cke haben
    while (sum + (H + 1) <= N) {
        H++;
        sum += H;
    }

    int R = N - sum;
    printf("%d %d\n", H, R);
    return 0;
}
