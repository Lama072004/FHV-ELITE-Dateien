#include "heart_condition.h"
#include <stdio.h>

// Tachykardie, Bradykardie und Arrhythmie Erkennung
void detect_heart_condition(float pulse_rate) {
    if (pulse_rate > 100) {
        printf("Tachykardie erkannt: Puls zu hoch!\n");
    } else if (pulse_rate < 60) {
        printf("Bradykardie erkannt: Puls zu niedrig!\n");
    } else {
        printf("Normale Pulsfrequenz: %.2f BPM\n", pulse_rate);
    }
}
