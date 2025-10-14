#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

char* IntToHex(uint32_t Int) {
    static char Hex[9];
    char BackwardsHex[9];
    uint32_t i = Int;
    uint32_t k = 0;

    // Spezialfall 0
    if (i == 0) {
        Hex[0] = '0';
        Hex[1] = '\0';
        return Hex;
    }

    // Umrechnen in Reste (rückwärts)
    while (i != 0 && k < 8) {
        BackwardsHex[k++] = i % 16;
        i = i / 16;
    }

    // Rückwärts in richtige Reihenfolge
    for (int temp = 0; temp < k; temp++) {
        uint8_t val = BackwardsHex[k - 1 - temp];

        if (val < 10)
            Hex[temp] = '0' + val;
        else if (val == 10)
            Hex[temp] = 'A';
        else if (val == 11)
            Hex[temp] = 'B';
        else if (val == 12)
            Hex[temp] = 'C';
        else if (val == 13)
            Hex[temp] = 'D';
        else if (val == 14)
            Hex[temp] = 'E';
        else if (val == 15)
            Hex[temp] = 'F';
    }

    Hex[k] = '\0'; // String beenden
    return Hex;
}

uint32_t HexToInt(void) {
    
    return 0;
}

int main(void){

    uint32_t Integer1 = 0;

    scanf_s("%d", &Integer1);
    char* Hex1 = IntToHex(Integer1);
    printf("Dezimal: %u -> Hex: %s\n", Integer1, Hex1);
    


    return 0;
}