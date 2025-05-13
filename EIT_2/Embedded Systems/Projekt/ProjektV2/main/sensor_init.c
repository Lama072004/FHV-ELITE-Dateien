#include "driver/i2c_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdint.h>

#define I2C_SDA_GPIO    5
#define I2C_SCL_GPIO    6
#define MAX30102_ADDR   0x57

static i2c_master_bus_handle_t i2c_bus = NULL;
static i2c_master_dev_handle_t max30102_dev = NULL;

// I2C initialisieren mit i2c_master API
void initI2C() {
    const i2c_master_bus_config_t bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .scl_io_num = I2C_SCL_GPIO,
        .sda_io_num = I2C_SDA_GPIO,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &i2c_bus));

    const i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = MAX30102_ADDR,
        .scl_speed_hz = 400000,
    };

    ESP_ERROR_CHECK(i2c_master_bus_add_device(i2c_bus, &dev_cfg, &max30102_dev));
}

// Ein Register schreiben
void writeRegister(uint8_t reg, uint8_t value) {
    uint8_t data[2] = {reg, value};
    ESP_ERROR_CHECK(i2c_master_transmit(max30102_dev, data, sizeof(data), pdMS_TO_TICKS(100)));
}

// Sensor initialisieren
void max30102_init() {
    writeRegister(0x09, 0x40);  // Reset
    vTaskDelay(pdMS_TO_TICKS(10));

    writeRegister(0x08, 0x4F);  // FIFO Config
    writeRegister(0x09, 0x03);  // Mode: SpO2
    writeRegister(0x0A, 0x27);  // SpO2 Config
    writeRegister(0x0C, 0x3F);  // RED LED
    writeRegister(0x0D, 0x3F);  // IR LED
}

// FIFO lesen
void max30102_read_fifo(uint32_t *red, uint32_t *ir) {
    uint8_t reg = 0x07;
    uint8_t data[6];
    ESP_ERROR_CHECK(i2c_master_transmit_receive(max30102_dev, &reg, 1, data, 6, pdMS_TO_TICKS(100)));

    *red = ((uint32_t)data[0] << 16) | ((uint32_t)data[1] << 8) | data[2];
    *ir  = ((uint32_t)data[3] << 16) | ((uint32_t)data[4] << 8) | data[5];
}
