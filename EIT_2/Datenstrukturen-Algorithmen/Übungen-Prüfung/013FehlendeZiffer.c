<<<<<<< HEAD
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define SumZahl1bis9 45

int main() {
    char Zahlen[10];
    int summe = 0;
    int FehlendeZahl;

    scanf("%s", Zahlen);

    for (int i = 0; i < 9; i++) {
        if (Zahlen[i] == '?') {
            continue;
        }
        summe += Zahlen[i] - '0'; //ASCII in Zahl umwandeln
    }

    FehlendeZahl = SumZahl1bis9 - summe;

    printf("Fehlende Zahl: %d\n", FehlendeZahl);

    return 0;
}
=======
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define SumZahl1bis9 45

int main() {
    char Zahlen[10];
    int summe = 0;
    int FehlendeZahl;

    scanf("%s", Zahlen);

    for (int i = 0; i < 9; i++) {
        if (Zahlen[i] == '?') {
            continue;
        }
        summe += Zahlen[i] - '0'; //ASCII in Zahl umwandeln
    }

    FehlendeZahl = SumZahl1bis9 - summe;

    printf("Fehlende Zahl: %d\n", FehlendeZahl);

    return 0;
}
>>>>>>> 23d3c8addaa54cb260e9830e82515419590b70cc
