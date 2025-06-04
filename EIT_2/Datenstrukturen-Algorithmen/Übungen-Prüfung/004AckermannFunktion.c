#include <stdio.h>

unsigned int Ackermann(unsigned int m, unsigned int n) {
    if (m == 0) {
        return n + 1;
    }
    else if (m > 0 && n == 0) {
        return Ackermann(m - 1, 1);
    }
    else { // m > 0 && n > 0
        return Ackermann(m - 1, Ackermann(m, n - 1));
    }
}

int main() {
    unsigned int m = 2;
    unsigned int n = 3;
    unsigned int A = Ackermann(m, n);
    printf("%u\n", A);
    return 0;
}
