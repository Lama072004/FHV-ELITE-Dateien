#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

void app_main(void)
{
    uint32_t last_second_msg = 0;      // Zeit für "Es ist eine Sekunde vergangen ..."
    uint32_t last_countdown1 = 0;      // Countdown 1 alle 1 s
    uint32_t last_countdown2 = 0;      // Countdown 2 alle 2 s

    int countdown1 = 10;
    int countdown2 = 15;

    while (1) {

        uint32_t now = xTaskGetTickCount();  // aktuelle Systemzeit in Ticks

        // --- 1. Jede Sekunde ---
        if (now - last_second_msg >= pdMS_TO_TICKS(1000)) {
            last_second_msg = now;
            printf("Es ist eine Sekunde vergangen ...\n");
        }

        // --- 2. Countdown 1: jede Sekunde ---
        if (countdown1 >= 0 && now - last_countdown1 >= pdMS_TO_TICKS(1000)) {
            last_countdown1 = now;
            printf("Countdown 1: %d\n", countdown1);
            countdown1--;
        }

        // --- 3. Countdown 2: alle 2 Sekunden ---
        if (countdown2 >= 0 && now - last_countdown2 >= pdMS_TO_TICKS(2000)) {
            last_countdown2 = now;
            printf("Countdown 2: %d\n", countdown2);
            countdown2--;
        }
    }
}
