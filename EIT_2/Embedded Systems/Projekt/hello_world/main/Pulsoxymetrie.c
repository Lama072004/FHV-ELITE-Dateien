#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "sdkconfig.h" // Für CONFIG_I2C_MASTER_SCL_IO und CONFIG_I2C_MASTER_SDA_IO

#define I2C_MASTER_SCL_IO  6    // Beispiel: SCL an GPIO8
#define I2C_MASTER_SDA_IO  5   // Beispiel: SDA an GPIO10

#define I2C_MASTER_NUM              0   // I2C-Port-Nummer
#define I2C_MASTER_FREQ_HZ      100000   // Frequenz: 100kHz
#define I2C_MASTER_TX_BUF_DISABLE   0    // Keine TX-Buffer
#define I2C_MASTER_RX_BUF_DISABLE   0    // Keine RX-Buffer

#define MAX30102_ADDR 0x57  // MAX30102 I2C-Adresse
#define TAG "MAX30102"      // Log Tag

esp_err_t i2c_master_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    esp_err_t err = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (err != ESP_OK) {
        return err;
    }
    return i2c_driver_install(I2C_MASTER_NUM, conf.mode,
                              I2C_MASTER_RX_BUF_DISABLE,
                              I2C_MASTER_TX_BUF_DISABLE, 0);
}

// --- Sensor Initialisierung ---
static esp_err_t max30102_init(void)
{
    esp_err_t err;

    // Reset
    uint8_t reset_cmd[2] = { 0x09, 0x40 };
    err = i2c_master_write_to_device(I2C_MASTER_NUM, MAX30102_ADDR, reset_cmd, sizeof(reset_cmd), pdMS_TO_TICKS(1000));
    if (err != ESP_OK) return err;
    vTaskDelay(pdMS_TO_TICKS(100));

    // Interrupts deaktivieren
    uint8_t int_enable[2] = { 0x02, 0x00 };
    err = i2c_master_write_to_device(I2C_MASTER_NUM, MAX30102_ADDR, int_enable, sizeof(int_enable), pdMS_TO_TICKS(1000));
    if (err != ESP_OK) return err;

    // FIFO Konfiguration
    uint8_t fifo_conf[2] = { 0x08, 0x4F };
    err = i2c_master_write_to_device(I2C_MASTER_NUM, MAX30102_ADDR, fifo_conf, sizeof(fifo_conf), pdMS_TO_TICKS(1000));
    if (err != ESP_OK) return err;

    // Mode Konfiguration (SpO2 Modus)
    uint8_t mode_conf[2] = { 0x09, 0x03 };
    err = i2c_master_write_to_device(I2C_MASTER_NUM, MAX30102_ADDR, mode_conf, sizeof(mode_conf), pdMS_TO_TICKS(1000));
    if (err != ESP_OK) return err;

    // SPO2-Konfiguration: 411Hz, 18bit
    uint8_t spo2_conf[2] = { 0x0A, 0x27 };
    err = i2c_master_write_to_device(I2C_MASTER_NUM, MAX30102_ADDR, spo2_conf, sizeof(spo2_conf), pdMS_TO_TICKS(1000));
    if (err != ESP_OK) return err;

    // LED Pulse Amplituden
    uint8_t led1_pa[2] = { 0x0C, 0x24 }; // RED
    uint8_t led2_pa[2] = { 0x0D, 0x24 }; // IR
    err = i2c_master_write_to_device(I2C_MASTER_NUM, MAX30102_ADDR, led1_pa, sizeof(led1_pa), pdMS_TO_TICKS(1000));
    if (err != ESP_OK) return err;
    err = i2c_master_write_to_device(I2C_MASTER_NUM, MAX30102_ADDR, led2_pa, sizeof(led2_pa), pdMS_TO_TICKS(1000));
    if (err != ESP_OK) return err;

    ESP_LOGI(TAG, "MAX30102 initialization complete!");
    return ESP_OK;
}

// --- FIFO Daten auslesen ---
static esp_err_t max30102_read_fifo(uint32_t *red_led, uint32_t *ir_led)
{
    uint8_t fifo_data[6];
    esp_err_t err = i2c_master_write_read_device(I2C_MASTER_NUM, MAX30102_ADDR, (uint8_t[]){0x07}, 1, fifo_data, 6, pdMS_TO_TICKS(1000));
    if (err != ESP_OK) return err;

    *red_led = ((fifo_data[0] & 0x03) << 16) | (fifo_data[1] << 8) | fifo_data[2];
    *ir_led  = ((fifo_data[3] & 0x03) << 16) | (fifo_data[4] << 8) | fifo_data[5];
    return ESP_OK;
}



// --- Haupttask ---
void app_main(void)
{
    ESP_LOGI(TAG, "Initializing I2C...");
    ESP_ERROR_CHECK(i2c_master_init());

    ESP_LOGI(TAG, "Initializing MAX30102...");
    ESP_ERROR_CHECK(max30102_init());

    uint32_t red_led = 0, ir_led = 0;

    while (1) {
        max30102_read_fifo(&red_led, &ir_led);

        printf("IR: %lu, RED: %lu", ir_led, red_led);
        printf("\n");

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
