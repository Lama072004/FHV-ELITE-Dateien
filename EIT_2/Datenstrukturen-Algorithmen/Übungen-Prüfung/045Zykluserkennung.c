#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXZAHLEN 300

int main() {
    int N;
    int A[MAXZAHLEN];
    bool visited[MAXZAHLEN] = { false };

    scanf("%d", &N);
    for (int i = 0; i < N; i++) {
        scanf("%d", &A[i]);
    }

    int current = A[0];
    visited[0] = true;

    while (!visited[current]) {
        if (current == 0) {
            printf("true\n");
            return 0;
        }

        visited[current] = true;
        current = A[current];
    }

    if (current == 0) {
        printf("true\n");
    }
    else {
        printf("false\n");
    }

    return 0;
}