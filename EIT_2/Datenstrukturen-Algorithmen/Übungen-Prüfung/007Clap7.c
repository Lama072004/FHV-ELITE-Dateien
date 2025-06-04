#include <stdio.h>
#include <ctype.h>

int quersumme(int n) {
    int sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

int enthaelt_7(int n) {
    while (n > 0) {
        if (n % 10 == 7)
            return 1;
        n /= 10;
    }
    return 0;
}

int main() {
    
    int N;
    int clap=0;

    scanf_s("%d", &N);

    for (int i = 1; i <= N; i++) {
        if (i % 7 == 0 || enthaelt_7(i) || quersumme(i) % 7 == 0)
            clap++;
    }
    printf("%d", clap);

    return 0;
}
