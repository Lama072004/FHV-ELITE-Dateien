#include <stdio.h>
#include <stdint.h>

// ============================
// 1. Call-by-Value / Reference
// ============================
typedef struct {
    int a;
    int b;
} MyStruct;

// Call-by-Value
void modifyStructByValue(MyStruct s) {
    s.a = 100;
    s.b = 200;
    // Änderungen wirken nur lokal
}

// Call-by-Reference
void modifyStructByReference(MyStruct* ps) {
    ps->a = 100;
    ps->b = 200;
}

// ============================
// 2. Bitmaskierung: Set / Clear Bit
// ============================
void setBit(uint32_t* pX, uint8_t n) {
    *pX |= (1UL << n);
}

void clearBit(uint32_t* pX, uint8_t n) {
    *pX &= ~(1UL << n);
}

// ============================
// 4. Bits zählen
// ============================
uint8_t countBits(uint64_t x) {
    uint8_t count = 0;
    while (x) {
        count += (x & 1);
        x >>= 1;
    }
    return count;
}

// ============================
// 5. getValue / setValue
// ============================
uint32_t getValue(uint32_t x, uint8_t n, uint8_t m) {
    uint32_t mask = ((1UL << m) - 1) << n;
    return (x & mask) >> n;
}

void setValue(uint32_t* pX, uint8_t n, uint8_t m, uint32_t v) {
    uint32_t mask = ((1UL << m) - 1) << n;
    *pX = (*pX & ~mask) | ((v << n) & mask);
}

// ============================
// Hauptprogramm
// ============================
int main(void) {
    int choice;
    printf("Waehle Aufgabe:\n");
    printf("1 - Call-by-Value / Reference\n");
    printf("2 - Bitmaskierung (Set/Clear Bit)\n");
    printf("4 - Bits zaehlen\n");
    printf("5 - getValue / setValue\n");
    printf("Eingabe: ");
    scanf_s("%d", &choice);

    switch (choice) {
    case 1: {
        MyStruct s = { 1, 2 };
        printf("Vorher: a = %d, b = %d\n", s.a, s.b);

        modifyStructByValue(s);
        printf("Nach Call-by-Value: a = %d, b = %d\n", s.a, s.b);

        modifyStructByReference(&s);
        printf("Nach Call-by-Reference: a = %d, b = %d\n", s.a, s.b);
        break;
    }

    case 2: {
        uint32_t x = 0x0;
        printf("Initial x = 0x%X\n", x);
        setBit(&x, 3);
        printf("Nach setBit(3): x = 0x%X\n", x);
        clearBit(&x, 3);
        printf("Nach clearBit(3): x = 0x%X\n", x);
        break;
    }

    case 4: {
        uint64_t x;
        printf("Gib eine Zahl ein: ");
        scanf_s("%llu", &x);
        uint8_t bits = countBits(x);
        printf("Anzahl gesetzter Bits: %u\n", bits);
        break;
    }

    case 5: {
        uint32_t x = 0x23;
        uint8_t n = 1;
        uint8_t m = 5;
        printf("x = 0x%X (binär 00100011)\n", x);

        uint32_t val = getValue(x, n, m);
        printf("getValue(x, %u, %u) = 0x%X\n", n, m, val);

        setValue(&x, n, m, 0x0F);
        printf("setValue(x, %u, %u, 0x0F) -> x = 0x%X\n", n, m, x);
        break;
    }

    default:
        printf("Ungültige Auswahl.\n");
        break;
    }

    return 0;
}
