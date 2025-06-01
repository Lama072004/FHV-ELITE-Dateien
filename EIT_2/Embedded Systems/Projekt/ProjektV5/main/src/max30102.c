#include "max30102.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// I2C-Adresse des MAX30102
#define MAX30102_ADDR       0x57

// Register-Adressen (Datenblatt entnommen)
#define REG_INTR_ENABLE_1   0x02
#define REG_FIFO_WR_PTR     0x04
#define REG_OVF_COUNTER     0x05
#define REG_FIFO_RD_PTR     0x06
#define REG_FIFO_DATA       0x07
#define REG_FIFO_CONFIG     0x08
#define REG_MODE_CONFIG     0x09
#define REG_SPO2_CONFIG     0x0A
#define REG_LED1_PA         0x0C
#define REG_LED2_PA         0x0D
#define REG_PART_ID         0xFF

static const char *TAG = "MAX30102";

// -------------------------------
// Hilfsfunktionen für I2C-Zugriff
// -------------------------------

// Ein einzelnes Register schreiben
static esp_err_t write_register(uint8_t reg, uint8_t value) {
    uint8_t data[2] = { reg, value };
    return i2c_master_write_to_device(I2C_MASTER_NUM, MAX30102_ADDR, data, 2, 100 / portTICK_PERIOD_MS);
}

// Ein Register lesen (oder mehrere Bytes ab Startadresse)
static esp_err_t read_register(uint8_t reg, uint8_t *data, size_t len) {
    return i2c_master_write_read_device(I2C_MASTER_NUM, MAX30102_ADDR, &reg, 1, data, len, 100 / portTICK_PERIOD_MS);
}

// ----------------------------
// Initialisierung des Sensors
// ----------------------------
void max30102_init(void) {
    uint8_t part_id = 0;

    // Sensor-ID prüfen
    read_register(REG_PART_ID, &part_id, 1);
    if (part_id != 0x15) {
        ESP_LOGE(TAG, "Nicht erkannter Sensor (ID: 0x%02X)", part_id);
        return;
    }

    // Soft-Reset durchführen
    write_register(REG_MODE_CONFIG, 0x40);
    vTaskDelay(pdMS_TO_TICKS(100));  // Warten auf Reset-Abschluss

    // FIFO-Register zurücksetzen
    write_register(REG_FIFO_WR_PTR, 0x00);
    write_register(REG_OVF_COUNTER, 0x00);
    write_register(REG_FIFO_RD_PTR, 0x00);

    // FIFO-Konfiguration:
    // Sample Averaging: 4 (0b010)
    // FIFO Rollover: deaktiviert (Bit 4 = 0)
    write_register(REG_FIFO_CONFIG, (0b010 << 5) | 0x00);

    // Betriebsmodus: SpO2-Modus (IR + RED)
    write_register(REG_MODE_CONFIG, 0x03);

    // SpO2-Konfiguration:
    // ADC Range: 4096 nA (0b10 << 5)
    // Sample Rate: 100 Hz (0b011 << 2)
    // LED Pulse Width: 411 µs / 18-bit (0b11)
    write_register(REG_SPO2_CONFIG, (0b10 << 5) | (0b011 << 2) | 0b11);

    // LED-Strom einstellen (max. 0xFF = 50 mA)
    write_register(REG_LED1_PA, 0x3F);  // Rote LED
    write_register(REG_LED2_PA, 0x3F);  // IR-LED

    // Interrupt aktivieren für "new FIFO data ready"
    write_register(REG_INTR_ENABLE_1, 0x40);  // Bit 6 = A_FULL_EN

    ESP_LOGI(TAG, "MAX30102 initialisiert");
}

// ----------------------------------------
// FIFO auslesen (je 3 Byte für IR und RED)
// ----------------------------------------
void max30102_read_fifo(uint32_t *red, uint32_t *ir) {
    uint8_t raw[6];
    read_register(REG_FIFO_DATA, raw, 6);

    // 18 Bit Werte extrahieren (jeweils 3 Byte, MSB zuerst)
    *red = ((raw[0] << 16) | (raw[1] << 8) | raw[2]) & 0x03FFFF;
    *ir  = ((raw[3] << 16) | (raw[4] << 8) | raw[5]) & 0x03FFFF;
}
