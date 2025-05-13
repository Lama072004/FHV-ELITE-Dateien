#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sensor_init.h"
#include "pulse.h"
#include "spo2.h"
#include "heart_condition.h"
// #include "bluetooth.h" // Falls später wieder benötigt

void app_main(void) {
    uint32_t red, ir;
    float pulse_rate = 0.0f;
    float spo2 = 0.0f;

    // I2C initialisieren und Sensor starten
    initI2C();
    max30102_init();

    while (1) {
        max30102_read_fifo(&red, &ir);
        printf("RED: %lu, IR: %lu\n", red, ir);

        // Berechnungen
        calculate_pulse(ir);
        pulse_rate = get_pulse_rate();
        spo2 = calculate_spo2(red, ir);

        int rounded_pulse = (int)(pulse_rate + 0.5f);
        int rounded_spo2 = (int)(spo2 + 0.5f);

        detect_heart_condition(rounded_pulse);
        // send_data_over_bluetooth(rounded_pulse, rounded_spo2); // Noch nicht aktiv

        vTaskDelay(pdMS_TO_TICKS(500)); // 500ms Delay
    }
}
