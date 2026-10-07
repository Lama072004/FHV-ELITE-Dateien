#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "sensor_init.h"
#include "sensor_read_write.h"
#include "heart.h"
#include "ble.h"

#define I2C_MASTER_SCL_IO          6
#define I2C_MASTER_SDA_IO          5
#define I2C_MASTER_NUM             I2C_NUM_0
#define I2C_MASTER_FREQ_HZ         400000
#define I2C_MASTER_TX_BUF_DISABLE  0
#define I2C_MASTER_RX_BUF_DISABLE  0

static const char *TAG = "MAIN_APP";

void i2c_master_init(void) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode,
                       I2C_MASTER_RX_BUF_DISABLE,
                       I2C_MASTER_TX_BUF_DISABLE, 0);
}

void ble_send_task(void *arg) {
    float spo2 = 0.0f;
    int bpm = 0;
    uint32_t red_buf[100], ir_buf[100];

    while (1) {
        // Sensorwerte sammeln
        for (int i = 0; i < 100; i++) {
            max30102_read_fifo(&red_buf[i], &ir_buf[i]);
            vTaskDelay(pdMS_TO_TICKS(10)); // 100 Hz Sampling
        }

        // SpO2 und Puls berechnen
        compute_spo2_and_heart_rate(red_buf, ir_buf, 100, &spo2, &bpm);

        if (spo2 > 0 && bpm > 10 && bpm < 250) {
            ESP_LOGI(TAG, "SpO2: %.1f%%, Puls: %d bpm", spo2, bpm);
            // Werte per BLE senden (z.B. Pulse als uint8_t, SpO2 als float)
        } else {
            ESP_LOGW(TAG, "Kein Finger erkannt oder ungültige Werte");
        }

        // 5 Sekunden warten bis nächste Messung und BLE-Sendung
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// --- Add test_notify_task as requested ---
void test_notify_task(void *param) {
    vTaskDelay(pdMS_TO_TICKS(2000)); // Wait for BLE connection
    ble_send_string("hello world");
    vTaskDelay(pdMS_TO_TICKS(2000));
    ble_send_string("HelloWorld");
    vTaskDelete(NULL);
}

void app_main(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "Starte I2C Bus...");
    i2c_master_init();

    ESP_LOGI(TAG, "Initialisiere Sensor...");
    max30102_init();

    ESP_LOGI(TAG, "Starte BLE...");
    ble_init();

    ESP_LOGI(TAG, "Starte BLE Notify Task...");
    xTaskCreate(ble_send_task, "ble_send_task", 4096, NULL, 5, NULL);

    // --- Start test_notify_task as well ---
    xTaskCreate(test_notify_task, "test_notify_task", 2048, NULL, 5, NULL);
}
