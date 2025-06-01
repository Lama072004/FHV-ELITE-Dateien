#include <stdio.h>
#include <math.h>
#include "driver/i2c.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

#define I2C_MASTER_SCL_IO          0
#define I2C_MASTER_SDA_IO          1
#define I2C_MASTER_NUM             I2C_NUM_0
#define I2C_MASTER_FREQ_HZ         400000
#define I2C_MASTER_TX_BUF_DISABLE  0
#define I2C_MASTER_RX_BUF_DISABLE  0
#define MAX30102_ADDR              0x57

static const char *TAG = "MAX30102_RAW";

// Register
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
#define REG_TEMP_INTEGER   0x1F
#define REG_TEMP_FRACTION  0x20
#define REG_PART_ID        0xFF

// I2C Hilfsfunktionen
esp_err_t write_register(uint8_t reg, uint8_t value) {
    uint8_t data[2] = { reg, value };
    return i2c_master_write_to_device(I2C_MASTER_NUM, MAX30102_ADDR, data, 2, 100 / portTICK_PERIOD_MS);
}

esp_err_t read_register(uint8_t reg, uint8_t *data, size_t len) {
    return i2c_master_write_read_device(I2C_MASTER_NUM, MAX30102_ADDR, &reg, 1, data, len, 100 / portTICK_PERIOD_MS);
}

void max30102_init() {
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

void max30102_read_fifo(uint32_t *red, uint32_t *ir) {
    uint8_t raw[6];
    read_register(REG_FIFO_DATA, raw, 6);
    *red = ((raw[0] << 16) | (raw[1] << 8) | raw[2]) & 0x03FFFF;
    *ir  = ((raw[3] << 16) | (raw[4] << 8) | raw[5]) & 0x03FFFF;
}

int hillclimb_peak_detection(uint32_t *data, int size, int min_distance, float threshold_ratio) {
    int peaks = 0;
    int last_peak = -min_distance;

    float max_val = 0;
    for (int i = 0; i < size; i++) {
        if (data[i] > max_val) max_val = data[i];
    }

    float threshold = max_val * threshold_ratio;

    for (int i = 1; i < size - 1; i++) {
        if (data[i] > threshold &&
            data[i] > data[i-1] &&
            data[i] > data[i+1] &&
            (i - last_peak) >= min_distance) {

            peaks++;
            last_peak = i;
        }
    }
    return peaks;
}

void compute_spo2_and_heart_rate(uint32_t *red_buf, uint32_t *ir_buf, int samples, float *spo2, int *bpm) {
    float red_dc = 0, ir_dc = 0;
    float red_ac = 0, ir_ac = 0;

    for (int i = 0; i < samples; i++) {
        red_dc += red_buf[i];
        ir_dc  += ir_buf[i];
    }
    red_dc /= samples;
    ir_dc  /= samples;

    for (int i = 0; i < samples; i++) {
        red_ac += fabsf((float)red_buf[i] - red_dc);
        ir_ac  += fabsf((float)ir_buf[i] - ir_dc);
    }
    red_ac /= samples;
    ir_ac  /= samples;

    if (ir_dc < 10000 || red_ac < 20 || ir_ac < 20) {
        *spo2 = 0.0f;
        *bpm = 0;
        return;
    }

    float R = (red_ac / red_dc) / (ir_ac / ir_dc);
    *spo2 = 110.0f - 25.0f * R;
    if (*spo2 > 100.0f) *spo2 = 100.0f;
    if (*spo2 < 0.0f)   *spo2 = 0.0f;

    int peaks = hillclimb_peak_detection(ir_buf, samples, 6, 0.6f); // Abstand = 6 Samples, Threshold = 60%
    *bpm = (int)(peaks * 60 / 10); // Annahme: 10 Sekunden Daten
}

void app_main(void) {
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

    max30102_init();

    uint32_t red_buf[100], ir_buf[100];
    float spo2 = 0.0f;
    int bpm = 0;

    while (1) {
        for (int i = 0; i < 100; i++) {
            max30102_read_fifo(&red_buf[i], &ir_buf[i]);
            vTaskDelay(pdMS_TO_TICKS(10)); // 100 Hz
        }

        compute_spo2_and_heart_rate(red_buf, ir_buf, 100, &spo2, &bpm);
        if (spo2 > 0 && bpm > 30 && bpm < 220) {
            ESP_LOGI(TAG, "SpO2: %.1f%%, Puls: %d bpm", spo2, bpm);
        } else {
            ESP_LOGW(TAG, "Kein Finger erkannt oder ungültige Werte");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}