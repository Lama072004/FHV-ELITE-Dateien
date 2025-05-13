#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "shtc3.h"

#define TAG "APP"

void app_main(void)
{
    initI2C();

    if (!shtc3_init()) {
        ESP_LOGE(TAG, "Sensorinitialisierung fehlgeschlagen!");
        return;
    }

    float temperature, humidity;

    while (1) {
        if (shtc3_read(&temperature, &humidity)) {
            ESP_LOGI(TAG, "Temperature: %.2f °C | Humidity: %.2f %%", temperature, humidity);
        } else {
            ESP_LOGW(TAG, "Fehler beim Lesen der Sensorwerte!");
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
