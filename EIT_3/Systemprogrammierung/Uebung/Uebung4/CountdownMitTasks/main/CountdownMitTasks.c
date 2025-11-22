#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// --- Task 1 ---
// Ausgabe jede Sekunde
void task_seconds(void *pvParameters)
{
    while (1) {
        printf("Es ist eine Sekunde vergangen ...\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// --- Task 2 ---
// Countdown 2: 15 → 0 alle 2 Sekunden
void task_countdown2(void *pvParameters)
{
    for (int i = 15; i >= 0; i--) {
        printf("Countdown 2: %d\n", i);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }

    // Task beenden
    vTaskDelete(NULL);
}

// --- Task 3 ---
// Countdown 1: 10 → 0 jede Sekunde
void task_countdown1(void *pvParameters)
{
    for (int i = 10; i >= 0; i--) {
        printf("Countdown 1: %d\n", i);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    // Task beenden
    vTaskDelete(NULL);
}

void app_main(void)
{
    // Task 1 (jede Sekunde)
    xTaskCreate(task_seconds, "task_seconds", 2048, NULL, 1, NULL);

    // Task 2 (alle 2 Sekunden)
    xTaskCreate(task_countdown2, "task_countdown2", 2048, NULL, 1, NULL);

    // Task 3 (jede Sekunde)
    xTaskCreate(task_countdown1, "task_countdown1", 2048, NULL, 1, NULL);
}
