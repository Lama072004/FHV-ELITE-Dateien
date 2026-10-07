#include <stdio.h>
#include "ble.h"
#include "heart.h"

void app_main(void)
{
    // Initialize the Non-Volatile Storage
    ble_init();

    // Start advertising
    esp_err_t ret = esp_ble_gap_start_advertising(&adv_params);
    if (ret) {
        printf("Failed to start advertising: %s\n", esp_err_to_name(ret));
    }

    // Main loop to read sensor data and send notifications
    while (1) {
        uint8_t pulse = 0; // Replace with actual pulse reading
        float spo2 = 0.0;  // Replace with actual SpO2 reading

        // Read sensor data
        read_sensor_data(&pulse, &spo2);

        // Send notifications
        ble_notify_values(pulse, spo2);

        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }
}