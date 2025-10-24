#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>

//Uebung-1
char* IntToHex(uint32_t Int) {
    static char Hex[9];
    char BackwardsHex[9];
    uint32_t i = Int;
    uint32_t k = 0;
    if (i == 0) {
        Hex[0] = '0';
        Hex[1] = '\0';
        return Hex;
    }
    while (i != 0 && k < 8) {
        BackwardsHex[k++] = i % 16;
        i /= 16;
    }
    for (int temp = 0; temp < k; temp++) {
        uint8_t val = BackwardsHex[k - 1 - temp];
        Hex[temp] = (val < 10) ? ('0' + val) : ('A' + (val - 10));
    }
    Hex[k] = '\0';
    return Hex;
}

//Uebung-2
uint32_t HexToIntIndexed(const char* hexStr) {
    uint32_t result = 0;
    for (size_t i = 0; hexStr[i] != '\0'; i++) {
        char c = toupper(hexStr[i]);
        uint8_t val = 0;
        if (c >= '0' && c <= '9') val = c - '0';
        else if (c >= 'A' && c <= 'F') val = c - 'A' + 10;
        else break;
        result = result * 16 + val;
    }
    return result;
}

//Uebung-3
uint32_t HexToIntPointer(const char* hexStr) {
    uint32_t result = 0;
    while (*hexStr != '\0') {
        char c = toupper(*hexStr++);
        uint8_t val = 0;
        if (c >= '0' && c <= '9') val = c - '0';
        else if (c >= 'A' && c <= 'F') val = c - 'A' + 10;
        else break;
        result = result * 16 + val;
    }
    return result;
}

//Uebung-4
char* IntToBase(uint32_t num, uint8_t base) {
    static char str[33];
    char rev[33];
    uint32_t i = 0;
    if (num == 0) { str[0] = '0'; str[1] = '\0'; return str; }
    while (num > 0) {
        uint8_t r = num % base;
        rev[i++] = (r < 10) ? ('0' + r) : ('A' + r - 10);
        num /= base;
    }
    for (uint32_t j = 0; j < i; j++) str[j] = rev[i - 1 - j];
    str[i] = '\0';
    return str;
}

//Uebung-4
uint32_t BaseToInt(const char* str, uint8_t base) {
    uint32_t result = 0;
    while (*str != '\0') {
        char c = toupper(*str++);
        uint8_t val = (c >= '0' && c <= '9') ? (c - '0') : (c - 'A' + 10);
        if (val >= base) break;
        result = result * base + val;
    }
    return result;
}

//Uebung-6
typedef struct {
    float temperatur;
    float luftfeuchtigkeit;
    float windgeschwindigkeit;
} WeatherData;

#define MAX_WEATHER 10
WeatherData weatherBuffer[MAX_WEATHER];
uint8_t weatherIndex = 0;

//Uebung-6
void appendWeatherData(WeatherData data) {
    weatherBuffer[weatherIndex++] = data;
    if (weatherIndex >= MAX_WEATHER) weatherIndex = 0;
}

//Uebung-7
typedef struct {
    float min;
    float max;
    float mean;
} WeatherStats;

//Uebung-7
void calculateWeatherStatsRef(WeatherData* data, uint8_t size, WeatherStats* stats) {
    if (size == 0) return;
    stats->min = stats->max = data[0].temperatur;
    float sum = 0;
    for (uint8_t i = 0; i < size; i++) {
        float val = data[i].temperatur;
        if (val < stats->min) stats->min = val;
        if (val > stats->max) stats->max = val;
        sum += val;
    }
    stats->mean = sum / size;
}

//Uebung-7
WeatherStats calculateWeatherStatsStruct(WeatherData* data, uint8_t size) {
    WeatherStats stats = { 0 };
    if (size == 0) return stats;
    stats.min = stats.max = data[0].temperatur;
    float sum = 0;
    for (uint8_t i = 0; i < size; i++) {
        float val = data[i].temperatur;
        if (val < stats.min) stats.min = val;
        if (val > stats.max) stats.max = val;
        sum += val;
    }
    stats.mean = sum / size;
    return stats;
}

//Uebung-Hauptprogramm
int main(void)
{
    int32_t Uebung = 0;
    printf(
        "Umwandlung Integer zu Hexadezimal:\t\t 1\n"
        "Umwandlung Hexadezimal zu Integer indiziert:\t 2\n"
        "Umwandlung Hexadezimal zu Integer, Pointer:\t 3\n"
        "Umwandlung mit beliebiger Basis [optional]:\t 4\n"
        "Hoelle, nein!:\t\t\t\t\t 5\n"
        "Wetterdaten:\t\t\t\t\t 6\n"
        "Wetterdatenauswertung:\t\t\t\t 7\n"
    );
    scanf_s("%d", &Uebung);

    switch (Uebung) {
    case 1: {
        uint32_t Integer = 0;
        printf("Geben Sie eine Zahl ein: ");
        scanf_s("%u", &Integer);
        char* Hex = IntToHex(Integer);
        printf("Dezimal: %u -> Hex: %s\n", Integer, Hex);
        break;
    }
    case 2: {
        char hexStr[9];
        printf("Geben Sie eine Hex-Zahl ein: ");
        scanf_s("%8s", hexStr, (unsigned)_countof(hexStr));
        uint32_t value = HexToIntIndexed(hexStr);
        printf("Hex: %s -> Dezimal: %u\n", hexStr, value);
        break;
    }
    case 3: {
        char hexStr[9];
        printf("Geben Sie eine Hex-Zahl ein: ");
        scanf_s("%8s", hexStr, (unsigned)_countof(hexStr));
        uint32_t value = HexToIntPointer(hexStr);
        printf("Hex: %s -> Dezimal: %u\n", hexStr, value);
        break;
    }
    case 4: {
        uint32_t Zahl;
        uint8_t Basis;
        printf("Geben Sie eine Zahl ein: ");
        scanf_s("%u", &Zahl);
        printf("Geben Sie die Basis ein (2-36): ");
        scanf_s("%hhu", &Basis);
        char* str = IntToBase(Zahl, Basis);
        printf("Zahl in Basis %d: %s\n", Basis, str);
        char inputStr[33];
        printf("Geben Sie eine Zahl in Basis %d ein: ", Basis);
        scanf_s("%32s", inputStr, (unsigned)_countof(inputStr));
        uint32_t wert = BaseToInt(inputStr, Basis);
        printf("Zurück zu Dezimal: %u\n", wert);
        break;
    }
    case 5: {
        int arr[3] = { 1,2,3 };
        for (int i = -1; i < 4; i++) printf("arr[%d] = %d\n", i, arr[i]);
        struct { int a; char b; float c; } s = { 1,'X',3.14f };
        printf("Adresse: a=%p b=%p c=%p\n", &s.a, &s.b, &s.c);
        uint32_t var = 0x12345678;
        uint8_t* p = (uint8_t*)&var;
        for (int i = 0; i < 4; i++) printf("Byte %d = %X\n", i, p[i]);
        break;
    }
    case 6: {
        for (int i = 0; i < 12; i++) {
            WeatherData wd = { 20.0f + i, 50.0f + i, 5.0f + i };
            appendWeatherData(wd);
        }
        for (int i = 0; i < MAX_WEATHER; i++)
            printf("%d: T=%.1f H=%.1f W=%.1f\n", i, weatherBuffer[i].temperatur, weatherBuffer[i].luftfeuchtigkeit, weatherBuffer[i].windgeschwindigkeit);
        break;
    }
    case 7: {
        WeatherStats statsRef;
        calculateWeatherStatsRef(weatherBuffer, MAX_WEATHER, &statsRef);
        printf("Ref: Min=%.1f Max=%.1f Mean=%.1f\n", statsRef.min, statsRef.max, statsRef.mean);
        WeatherStats statsStruct = calculateWeatherStatsStruct(weatherBuffer, MAX_WEATHER);
        printf("Struct: Min=%.1f Max=%.1f Mean=%.1f\n", statsStruct.min, statsStruct.max, statsStruct.mean);
        break;
    }
    default:
        printf("Nicht implementiert!\n");
        break;
    }

    return 0;
}
