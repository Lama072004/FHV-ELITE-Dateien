#include "sensor_init.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define MAX30102_ADDR              0x57

#define REG_INTR_ENABLE_1  0x02
#define REG_FIFO_WR_PTR    0x04
#define REG_OVF_COUNTER    0x05
#define REG_FIFO_RD_PTR    0x06
#define REG_FIFO_DATA      0x07
#define REG_FIFO_CONFIG    0x08
#define REG_MODE_CONFIG    0x09
#define REG_SPO2_CONFIG    0x0A
#define REG_LED1_PA        0x0C
#define REG_LED2_PA        0x0D
#define REG_PART_ID        0xFF

static const char *TAG = "MAX30102_RAW";

esp_err_t write_register(uint8_t reg, uint8_t value) {
    uint8_t data[2] = { reg, value };
    return i2c_master_write_to_device(I2C_NUM_0, MAX30102_ADDR, data, 2, 100 / portTICK_PERIOD_MS);
}

esp_err_t read_register(uint8_t reg, uint8_t *data, size_t len) {
    return i2c_master_write_read_device(I2C_NUM_0, MAX30102_ADDR, &reg, 1, data, len, 100 / portTICK_PERIOD_MS);
}

void max30102_init(void) {
    uint8_t part_id = 0;
    read_register(REG_PART_ID, &part_id, 1);
    if (part_id != 0x15) {
        ESP_LOGE(TAG, "Nicht erkannter Sensor (ID: 0x%02X)", part_id);
        return;
    }

    write_register(REG_MODE_CONFIG, 0x40); // Reset
    vTaskDelay(pdMS_TO_TICKS(100));

    write_register(REG_FIFO_WR_PTR, 0x00);
    write_register(REG_OVF_COUNTER, 0x00);
    write_register(REG_FIFO_RD_PTR, 0x00);
    write_register(REG_FIFO_CONFIG, (0b010 << 5) | 0x00); // Durchschnitt = 4 Samples
    write_register(REG_MODE_CONFIG, 0x03); // SpO2 Modus
    write_register(REG_SPO2_CONFIG, (0b10 << 5) | (0b11 << 2) | 0b11); // 100Hz, 411us, 32768nA
    write_register(REG_LED1_PA, 0x3F); // RED
    write_register(REG_LED2_PA, 0x3F); // IR
    write_register(REG_INTR_ENABLE_1, 0x40);

    ESP_LOGI(TAG, "MAX30102 initialisiert");
}
