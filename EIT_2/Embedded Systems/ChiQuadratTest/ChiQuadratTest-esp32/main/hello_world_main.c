#include <stdio.h>
#include <inttypes.h>
#include <math.h>

#define RNG_Base 0x60026000
#define RNG_Data_REG_Offs 0xB0

#define MIN_VALUE 1
#define MAX_VALUE 20
#define AMOUNT_NUMBERS 100

uint32_t read_trng(void) {
    return *(volatile uint32_t *)(RNG_Base + RNG_Data_REG_Offs);
}

uint32_t get_rand_in_range(uint32_t min, uint32_t max) {
    uint32_t random_num = read_trng();
    return min + (random_num % (max - min + 1));
}

void chi_square_test(uint32_t samples[], int num_samples) {
    int observed[MAX_VALUE - MIN_VALUE + 1] = {0};  // Initialisiert mit 0
    int num_classes = MAX_VALUE - MIN_VALUE + 1;    // Anzahl an Zahlen

    // 1. Zähle Häufigkeiten
    for (int i = 0; i < num_samples; i++) {
        uint32_t num = samples[i];
        observed[num - MIN_VALUE]++;
    }

    // 2. Berechne erwartete Häufigkeit
    double expected = (double)num_samples / num_classes;

    // 3. Berechne Chi-Quadrat
    double chi_square = 0.0;
    for (int i = 0; i < num_classes; i++) {
        chi_square += pow(observed[i] - expected, 2) / expected;
    }

    // gib Chi-Quadrat aus
    printf("\n%f\n", chi_square);
}

void app_main(void) {
    uint32_t random_numbers[AMOUNT_NUMBERS];

    // Generiere und drucke Zahlen
    for (int i = 0; i < AMOUNT_NUMBERS; i++) {
        random_numbers[i] = get_rand_in_range(MIN_VALUE, MAX_VALUE);
        printf("%" PRIu32 " ", random_numbers[i]);
    }

    // Führe Chi-Quadrat-Test durch
    chi_square_test(random_numbers, AMOUNT_NUMBERS);
}