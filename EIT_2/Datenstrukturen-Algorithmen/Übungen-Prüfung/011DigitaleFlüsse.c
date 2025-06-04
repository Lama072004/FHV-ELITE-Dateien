#include <stdio.h>

int Quersumme(int n) {
    int sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

int NaechsteZahl(int m) {
    return m + Quersumme(m);
}

int main() {
    int r1, r2;

    scanf_s("%d\n%d", &r1, &r2);

    
    while (r1 != r2) {
        if (r1 < r2) {
            r1 = NaechsteZahl(r1);
        } else {
            r2 = NaechsteZahl(r2);
        }
    }
    
    printf("%d", NaechsteZahl(r1));

    return 0;
}
