#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

int main() {
    char Befehle[250];
    char letzterBefehl[10] = "";  // Puffer für letzten Befehl
    int letzterBefehlCount = 0;

    fgets(Befehle, sizeof(Befehle), stdin);

    char* wort = strtok(Befehle, " ");

    while (wort != NULL) {
        if (strcmp(wort, letzterBefehl) == 0) {
            letzterBefehlCount++;
        }
        else {
            if (letzterBefehlCount > 0) {
                if (strcmp(letzterBefehl, "up") == 0) printf("^");
                else if (strcmp(letzterBefehl, "down") == 0) printf("v");
                else if (strcmp(letzterBefehl, "left") == 0) printf("<");
                else if (strcmp(letzterBefehl, "right") == 0) printf(">");
                if (letzterBefehlCount > 1) printf("%d", letzterBefehlCount);
            }
            strcpy(letzterBefehl, wort);
            letzterBefehlCount = 1;
        }

        wort = strtok(NULL, " ");
    }

    // Letzten Befehl ausgeben
    if (letzterBefehlCount > 0) {
        if (strcmp(letzterBefehl, "up") == 0) printf("^");
        else if (strcmp(letzterBefehl, "down") == 0) printf("v");
        else if (strcmp(letzterBefehl, "left") == 0) printf("<");
        else if (strcmp(letzterBefehl, "right") == 0) printf(">");
        if (letzterBefehlCount > 1) printf("%d", letzterBefehlCount);
    }

    printf("\n");
    return 0;
}
