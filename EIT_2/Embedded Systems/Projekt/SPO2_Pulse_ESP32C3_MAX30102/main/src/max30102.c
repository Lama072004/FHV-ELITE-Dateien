#include "max30102.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// I2C-Adresse des Sensors (festgelegt laut Datenblatt)
#define MAX30102_ADDR       0x57

// Wichtige Registeradressen laut Datenblatt
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

static const char *TAG = "MAX30102";  // Für Log-Ausgaben

// ------------------------------------
// I2C Hilfsfunktionen (Write + Read)
// ------------------------------------

// Einzelnes Register schreiben
static esp_err_t write_register(uint8_t reg, uint8_t value) {
    uint8_t data[2] = { reg, value };
    return i2c_master_write_to_device(I2C_MASTER_NUM, MAX30102_ADDR, data, 2, 100 / portTICK_PERIOD_MS);
}

// Register lesen (kann auch mehrere Bytes ab Startadresse lesen)
static esp_err_t read_register(uint8_t reg, uint8_t *data, size_t len) {
    return i2c_master_write_read_device(I2C_MASTER_NUM, MAX30102_ADDR, &reg, 1, data, len, 100 / portTICK_PERIOD_MS);
}

// ----------------------------------------
// Initialisiert den MAX30102 für SpO2-Messung
// ----------------------------------------
void max30102_init(void) {
    uint8_t part_id = 0;

    // Prüfen, ob der richtige Sensor angeschlossen ist
    read_register(REG_PART_ID, &part_id, 1);
    if (part_id != 0x15) {
        ESP_LOGE(TAG, "Nicht erkannter Sensor (ID: 0x%02X)", part_id);
        return;
    }

    // Soft-Reset des Sensors
    write_register(REG_MODE_CONFIG, 0x40);
    vTaskDelay(pdMS_TO_TICKS(100));  // Warten auf Abschluss

    // FIFO zurücksetzen: Schreib-, Lese- und Überlaufzeiger
    write_register(REG_FIFO_WR_PTR, 0x00);
    write_register(REG_OVF_COUNTER, 0x00);
    write_register(REG_FIFO_RD_PTR, 0x00);

    // FIFO-Konfiguration:
    // Sample-Averaging = 4 Samples mitteln (bessere Glättung)
    // Rollover deaktiviert (FIFO bleibt leer bei Überlauf)
    write_register(REG_FIFO_CONFIG, (0b010 << 5) | 0x00);

    // Betriebsmodus: SpO2-Modus → Rote + IR-LED aktiv
    write_register(REG_MODE_CONFIG, 0x03);

    // SpO2-Konfiguration:
    // ADC Full Scale Range: 4096 nA
    // Sample Rate: 100 Hz
    // LED-Pulsweite: 411 µs (entspricht 18-Bit-Auflösung)
    write_register(REG_SPO2_CONFIG, (0b10 << 5) | (0b011 << 2) | 0b11);

    // LED-Strom für beide Kanäle setzen (0x3F ≈ 19.5 mA)
    write_register(REG_LED1_PA, 0x3F);  // Rote LED
    write_register(REG_LED2_PA, 0x3F);  // IR-LED

    // Interrupt aktivieren: FIFO-Daten voll (Bit 6 = A_FULL_EN)
    write_register(REG_INTR_ENABLE_1, 0x40);

    ESP_LOGI(TAG, "MAX30102 initialisiert");
}

// ----------------------------------------------------
// FIFO-Daten lesen: je 3 Byte pro Kanal (18-Bit-Werte)
// ----------------------------------------------------
void max30102_read_fifo(uint32_t *red, uint32_t *ir) {
    uint8_t raw[6];  // 3 Byte für RED, 3 Byte für IR
    read_register(REG_FIFO_DATA, raw, 6);

    // Extrahiere 18-Bit-Werte: [23:6] gültig, MSB zuerst
    *red = ((raw[0] << 16) | (raw[1] << 8) | raw[2]) & 0x03FFFF;
    *ir  = ((raw[3] << 16) | (raw[4] << 8) | raw[5]) & 0x03FFFF;
}
