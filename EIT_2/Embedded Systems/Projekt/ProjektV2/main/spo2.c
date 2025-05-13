#include "spo2.h"
#include <stdint.h>
#include <math.h>

float calculate_spo2(uint32_t red_value, uint32_t ir_value) {
    if (ir_value == 0) return 0.0f;  // Division durch 0 verhindern

    float ratio = (float)red_value / (float)ir_value;
    float spo2 = 110.0f - 25.0f * ratio;

    // Begrenzung auf 0–100 %
    if (spo2 > 100.0f) spo2 = 100.0f;
    if (spo2 < 0.0f) spo2 = 0.0f;

    return roundf(spo2);  // Auf ganze Prozent runden
}
