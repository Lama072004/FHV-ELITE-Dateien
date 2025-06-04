#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>



int main() {
    char oben_links, oben_rechts;
    char mitte_links, mitte_rechts;
    char unten_links, unten_rechts;

    // Eingabe einlesen
    scanf(" %c%c", &oben_links, &oben_rechts);
    scanf(" %c%c", &mitte_links, &mitte_rechts);
    scanf(" %c%c", &unten_links, &unten_rechts);

    bool sicher = true;

    // eigenes Auto in der Mitte links
    char eigenes = mitte_links;

    // Wenn rechts direkt daneben ein Auto mit gleicher Geschwindigkeit ist
    if (mitte_rechts != '|' && mitte_rechts == eigenes)
        sicher = false;

    // Wenn rechts vorne ein Auto ist, das langsamer ist
    if (oben_rechts != '|' && oben_rechts < eigenes)
        sicher = false;

    // Wenn rechts hinten ein Auto ist, das schneller ist
    if (unten_rechts != '|' && unten_rechts > eigenes)
        sicher = false;

    // Ausgabe
    printf(sicher ? "true\n" : "false\n");

    return 0;
}
