#include <stdio.h>
#include <math.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"

#include "max30102.h"   // Funktionen zur Initialisierung und Datenerfassung vom MAX30102-Sensor
#include "heart.h"      // Funktionen zur Berechnung von Pulsfrequenz und Sauerstoffsättigung (SpO2)

// Logging-Tag für ESP-IDF Logging-Makros (ESP_LOGI, ESP_LOGW, etc.)
static const char *TAG = "MAX30102_RAW";

// Hauptfunktion (wird beim Start ausgeführt)
void app_main(void) {
    // I2C-Konfigurationsstruktur initialisieren (legt Kommunikationsparameter fest)
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,                    // Betrieb im Master-Modus
        .sda_io_num = I2C_MASTER_SDA_IO,            // SDA-Pin
        .scl_io_num = I2C_MASTER_SCL_IO,            // SCL-Pin
        .sda_pullup_en = GPIO_PULLUP_ENABLE,        // Interner Pull-Up-Widerstand für SDA aktivieren
        .scl_pullup_en = GPIO_PULLUP_ENABLE,        // Interner Pull-Up-Widerstand für SCL aktivieren
        .master.clk_speed = I2C_MASTER_FREQ_HZ,     // Taktfrequenz für I2C-Kommunikation
    };

    // I2C-Parameter anwenden
    i2c_param_config(I2C_MASTER_NUM, &conf);
    // I2C-Treiber installieren (kein RX/TX-Buffer nötig → 0,0)
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);

    // MAX30102-Sensor initialisieren (inkl. interner Einstellungen)
    max30102_init();

    // Datenpuffer für 100 Samples (je Kanal Rotlicht & Infrarotlicht)
    uint32_t red_buf[100], ir_buf[100];
    float spo2 = 0.0f;    // Variable zur Speicherung der Sauerstoffsättigung (SpO2)
    int bpm = 0;          // Variable zur Speicherung der Herzfrequenz (Schläge pro Minute)

    while (1) {
        // 100 Rohdatenpaare vom Sensor lesen (Abstand 10 ms → ca. 100 Hz Samplingrate)
        for (int i = 0; i < 100; i++) {
            max30102_read_fifo(&red_buf[i], &ir_buf[i]);  // Einzelnes Sample lesen (rot + IR)
            vTaskDelay(pdMS_TO_TICKS(10));                // 10 ms warten zwischen Samples
        }

        // Verarbeitung der Rohdaten → Bestimmung von SpO2 und Herzfrequenz
        compute_spo2_and_heart_rate(red_buf, ir_buf, 100, &spo2, &bpm);

        // Nur plausible Ergebnisse ausgeben (SpO2 > 60 % → Finger erkannt)
        if (spo2 > 60) {
            ESP_LOGI(TAG, "SpO2: %.1f%%, Puls: %d bpm", spo2, bpm);
        } else {
            ESP_LOGW(TAG, "Kein Finger erkannt oder ungültige Werte");
        }

        // Eine Sekunde warten, bevor der nächste Messzyklus beginnt
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
