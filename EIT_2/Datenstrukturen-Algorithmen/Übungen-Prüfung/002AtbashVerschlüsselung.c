#include <stdio.h>

int main()
{
    char Wort[250];
    char WortNeu[250];
    char ch;
    int i;

    // Zeichenweise einlesen, bis Enter oder max. 249 Zeichen erreicht
    for (i = 0; i < 249; i++) {
        scanf_s("%c", &ch, 1);
        if (ch == '\n') {
            break;
        }
        Wort[i] = ch;
    }
    Wort[i] = '\0'; // Nullterminator für String-Ende
    
    for (int i = 0; i < 249; i++) {
        WortNeu[i] = 'z' - (Wort[i] - 'a');
        printf("%c", WortNeu[i]);
    }
    
    return 0;
}
