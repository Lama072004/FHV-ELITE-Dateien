#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdbool.h>

static uint32_t previous_ir_value = 0;
static bool pulse_detected = false;
static uint32_t pulse_start_time = 0;
static uint32_t pulse_interval = 0;
static float smoothed_bpm = 0.0f;

void calculate_pulse(uint32_t ir_value) {
    const uint32_t threshold = 100000; // Einfacher Schwellenwert
    uint32_t now = xTaskGetTickCount();

    if (!pulse_detected && ir_value > previous_ir_value + threshold) {
        // Puls erkannt
        pulse_detected = true;
        if (pulse_start_time > 0) {
            pulse_interval = now - pulse_start_time;
        }
        pulse_start_time = now;
    } else if (pulse_detected && ir_value < previous_ir_value) {
        pulse_detected = false;
    }

    previous_ir_value = ir_value;
}

float get_pulse_rate() {
    if (pulse_interval > 0) {
        float bpm = 60000.0f / ((float)pulse_interval * portTICK_PERIOD_MS);
        // Glättung
        smoothed_bpm = 0.7f * smoothed_bpm + 0.3f * bpm;
        return smoothed_bpm;
    }
    return 0.0f;
}
