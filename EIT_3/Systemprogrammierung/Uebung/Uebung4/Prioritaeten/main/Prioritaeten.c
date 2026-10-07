#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"

static const char *TAG = "PRIO_TEST";

// ------------------------------------------------------
// performCalculation
// ------------------------------------------------------
void performCalculation(void *param)
{
    int64_t start = esp_timer_get_time(); // µs

    // Dummy-Rechenlast
    for (volatile int i = 0; i < 100000; i++) {
        // ...
    }

    int64_t end = esp_timer_get_time();

    const char *name = pcTaskGetName(NULL);

    ESP_LOGI(TAG,
             "Calculation of task '%s' done — time: %lld us",
             name, (long long)(end - start));

    vTaskDelete(NULL);
}

// ------------------------------------------------------
// app_main (Prio wird auf 4 gesetzt)
// ------------------------------------------------------
void app_main(void)
{
    // app_main Priorität erhöhen
    // (app_main läuft als Task, Handle kann so geholt werden)
    TaskHandle_t mainHandle = xTaskGetCurrentTaskHandle();
    vTaskPrioritySet(mainHandle, 4);

    ESP_LOGI(TAG, "app_main priority set to %d", uxTaskPriorityGet(mainHandle));

    // Task A → Prio 3
    xTaskCreate(
        performCalculation,
        "calc_low",
        2048,
        NULL,
        3,
        NULL);

    // Task B → Prio 5
    xTaskCreate(
        performCalculation,
        "calc_high",
        2048,
        NULL,
        5,
        NULL);

    // app_main führt Calculation selbst aus
    performCalculation(NULL);

    // Warten damit Ausgabe sichtbar bleibt
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
