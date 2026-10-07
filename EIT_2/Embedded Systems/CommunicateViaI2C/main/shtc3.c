#include "driver/i2c.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <stdint.h>

#define I2C_NUM         I2C_NUM_0
#define SDA_PIN         GPIO_NUM_5
#define SCL_PIN         GPIO_NUM_6
#define SHTC3_ADDR      0x70  // 7-bit Adresse

#define TAG "SHTC3"

#define CMD_WAKEUP             0x3517
#define CMD_SLEEP              0xB098
#define CMD_SOFT_RESET         0x805D
#define CMD_MEASURE_T_FIRST    0x7866  // Read T first, no clock stretching
#define CMD_READ_ID            0xEFC8

static esp_err_t shtc3_write_command(uint16_t cmd) {
    uint8_t data[2] = { cmd >> 8, cmd & 0xFF };
    return i2c_master_write_to_device(I2C_NUM, SHTC3_ADDR, data, 2, pdMS_TO_TICKS(100));
}

void initI2C() {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = SDA_PIN,
        .scl_io_num = SCL_PIN,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000
    };
    i2c_param_config(I2C_NUM, &conf);
    i2c_driver_install(I2C_NUM, conf.mode, 0, 0, 0);
}

static uint8_t shtc3_crc8(const uint8_t *data) {
    uint8_t crc = 0xFF;
    for (int i = 0; i < 2; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x31;
            else
                crc <<= 1;
        }
    }
    return crc;
}

bool shtc3_init() {
    shtc3_write_command(CMD_WAKEUP);
    vTaskDelay(pdMS_TO_TICKS(1));

    if (shtc3_write_command(CMD_READ_ID) != ESP_OK) {
        ESP_LOGE(TAG, "Sensor antwortet nicht");
        return false;
    }

    uint8_t id[3];
    if (i2c_master_read_from_device(I2C_NUM, SHTC3_ADDR, id, 3, pdMS_TO_TICKS(100)) != ESP_OK) {
        ESP_LOGE(TAG, "ID konnte nicht gelesen werden");
        return false;
    }

    uint16_t product_id = (id[0] << 8) | id[1];
    ESP_LOGD(TAG, "Sensor ID: 0x%04X", product_id);

    return true;
}

bool shtc3_read(float *temperature, float *humidity) {
    shtc3_write_command(CMD_WAKEUP);
    vTaskDelay(pdMS_TO_TICKS(1));

    if (shtc3_write_command(CMD_MEASURE_T_FIRST) != ESP_OK) {
        ESP_LOGW(TAG, "CMD_MEASURE_T_FIRST fehlgeschlagen");
        return false;
    }

    vTaskDelay(pdMS_TO_TICKS(25));  // Warten auf Messung

    uint8_t data[6];
    if (i2c_master_read_from_device(I2C_NUM, SHTC3_ADDR, data, 6, pdMS_TO_TICKS(100)) != ESP_OK) {
        ESP_LOGW(TAG, "I2C-Lesevorgang fehlgeschlagen");
        return false;
    }

    if (shtc3_crc8(&data[0]) != data[2]) {
        ESP_LOGW(TAG, "CRC-Fehler bei Temperatur: %02X %02X != %02X", data[0], data[1], data[2]);
        return false;
    }

    if (shtc3_crc8(&data[3]) != data[5]) {
        ESP_LOGW(TAG, "CRC-Fehler bei Feuchtigkeit: %02X %02X != %02X", data[3], data[4], data[5]);
        return false;
    }

    uint16_t raw_temp = (data[0] << 8) | data[1];
    uint16_t raw_hum  = (data[3] << 8) | data[4];

    *temperature = -45.0 + 175.0 * ((float)raw_temp / 65535.0);
    *humidity    = 100.0 * ((float)raw_hum / 65535.0);

    return true;
}
