#include <stdio.h>

int middle(int x, int y, int z) {
    if ((x >= y && x <= z) || (x >= z && x <= y)) return x;
    if ((y >= x && y <= z) || (y >= z && y <= x)) return y;
    return z;
}

int main() {
    int N, X, Y, Z;
    int output[20];

    scanf_s("%d", &N);
       

    for (int i = 0; i < N; i++) {
        scanf_s("%d %d %d", &X, &Y, &Z);
        output[i] = middle(X, Y, Z);
    }

    for (int i = 0; i < N; i++) {
        printf("%d\n", output[i]);
    }
    return 0;
}
