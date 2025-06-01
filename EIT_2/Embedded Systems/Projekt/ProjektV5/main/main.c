#include <stdio.h>
#include <math.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"

#include "max30102.h"   // Sensor-spezifische Funktionen (Initialisierung, FIFO-Lesen)
#include "heart.h"      // Funktionen zur Berechnung von Puls und SpO2

// Logging-Tag für die Konsole
static const char *TAG = "MAX30102_RAW";

void app_main(void) {
    // I2C-Konfiguration vorbereiten (Pins, Frequenz, Pullups)
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,                    // Master-Modus
        .sda_io_num = I2C_MASTER_SDA_IO,            // SDA-Pin
        .scl_io_num = I2C_MASTER_SCL_IO,            // SCL-Pin
        .sda_pullup_en = GPIO_PULLUP_ENABLE,        // interner Pullup für SDA
        .scl_pullup_en = GPIO_PULLUP_ENABLE,        // interner Pullup für SCL
        .master.clk_speed = I2C_MASTER_FREQ_HZ,     // Taktfrequenz in Hz
    };

    // I2C-Schnittstelle konfigurieren
    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);

    // MAX30102 initialisieren (inkl. I2C-Adresse, LED-Modus etc.)
    max30102_init();

    // Puffer für die Messdaten (100 Samples für Rot- und IR-Kanal)
    uint32_t red_buf[100], ir_buf[100];
    float spo2 = 0.0f;    // Sauerstoffsättigung
    int bpm = 0;          // Puls in Schlägen pro Minute

    while (1) {
        // 100 Messwerte einlesen (jeweils Red + IR, mit 10 ms Abstand = 100 Hz Sampling)
        for (int i = 0; i < 100; i++) {
            max30102_read_fifo(&red_buf[i], &ir_buf[i]);  // Rohdaten aus FIFO lesen
            vTaskDelay(pdMS_TO_TICKS(10));                // 10 ms Delay → ~100 Hz
        }

        // SpO2 und Puls aus den Rohdaten berechnen
        compute_spo2_and_heart_rate(red_buf, ir_buf, 100, &spo2, &bpm);

        // Ausgabe nur bei plausiblen Werten (SpO2 > 60 % als Finger-Erkennung)
        if (spo2 > 60) {
            ESP_LOGI(TAG, "SpO2: %.1f%%, Puls: %d bpm", spo2, bpm);  // Info-Log
        } else {
            ESP_LOGW(TAG, "Kein Finger erkannt oder ungültige Werte");  // Warnung
        }

        // Eine Sekunde warten, bevor die nächste Messung beginnt
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
