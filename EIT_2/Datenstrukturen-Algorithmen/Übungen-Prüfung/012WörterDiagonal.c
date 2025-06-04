#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

 
int main() {
    int N;
    char Gitter[10][11];

    char haupt[11] = { 0 };
    char neben[11] = { 0 };

    scanf("%d", &N);

    for (int i = 0; i < N; i++){
        scanf("%s", &Gitter[i][0]);
    }

    for (int i = 0; i < N; i++) {
        haupt[i] = Gitter[i][i];
        neben[i] = Gitter[i][N - 1 - i];
    }

    printf("%s  %s", haupt, neben);

    return 0;
}
