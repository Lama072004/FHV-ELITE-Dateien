#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    char n[33];
    scanf("%[^\n]", n);

    int len = strlen(n);
    int i = len - 1;

    // Letzte Ziffer erhöhen und Überlauf behandeln
    while (i >= 0)
    {
        if (n[i] != '9') // Wenn Ziffer nicht 9 ist
        {
            n[i]++;
            break;
        }
        else // Wenn Ziffer 9 ist
        {
            n[i] = '0';
            i--;
        }
    }

    // Falls alle Ziffern 9 waren, neue Ziffer hinzufügen
    if (i == -1)
    {
        printf("1"); // Neue Ziffer '1' hinzufügen
        for (int j = 0; j < len; j++)
        {
            printf("1"); // Rest mit '1' füllen
        }
        printf("\n");
        return 0;
    }

    // Überprüfen, ob die Zahl wachsend ist
    for (i = 1; i < len; i++)
    {
        if (n[i] < n[i - 1]) // Wenn Ziffer kleiner als die vorherige ist
        {
            for (int j = i; j < len; j++) // Alle folgenden Ziffern anpassen
            {
                n[j] = n[i - 1]; // Auf den Wert der vorherigen Ziffer setzen
            }
            break; // Beenden
        }
    }

    // Erneut überprüfen, ob die Zahl wachsend ist
    for (i = 1; i < len; i++)
    {
        if (n[i] < n[i - 1]) // Wenn Ziffer kleiner als die vorherige ist
        {
            for (int j = i; j < len; j++) // Alle folgenden Ziffern anpassen
            {
                n[j] = n[0]; // Auf den Wert der ersten Ziffer setzen
            }
            break; // Beenden
        }
    }

    printf("%s\n", n);
    return 0;
}