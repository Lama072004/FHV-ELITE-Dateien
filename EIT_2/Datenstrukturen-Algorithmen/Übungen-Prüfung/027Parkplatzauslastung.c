#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#define MAXAUTOS 50

int main() {
    int anzahlAutos = MAXAUTOS;
    int startEndZeiten[MAXAUTOS * 2];
    bool Arbeitszeit[24] = { false };
    int startZeit = 0;
    int endZeit = 0;
    int Arbeitsstunden = 0;

    // Eingabe der tatsächlichen Anzahl Autos
    scanf("%d", &anzahlAutos);

    // Eingabe der Start- und Endzeiten
    for (int i = 0; i < (anzahlAutos * 2); i += 2) {
        scanf("%d %d", &startEndZeiten[i], &startEndZeiten[i + 1]);
    }

    // Markiere belegte Arbeitsstunden
    for (int j = 0; j < (anzahlAutos * 2); j += 2) {
        startZeit = startEndZeiten[j];
        endZeit = startEndZeiten[j + 1];

        // Stunden im Bereich markieren
        for (int k = startZeit; k < endZeit && k < 24; k++) {
            Arbeitszeit[k] = true;
        }
    }

    // Berechnung Arbeitsstunden
    for (int i = 0; i < 24; i++) {
        Arbeitsstunden += Arbeitszeit[i];
    }
    printf("%d Arbeitsstunden", Arbeitsstunden);
    return 0;
}
