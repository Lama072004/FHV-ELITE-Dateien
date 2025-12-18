/*
 * ============================================================================
 * DIGITALE WAAGE MIT BLUETOOTH
 * ============================================================================
 * 
 * Beschreibung:
 * Dieses Programm implementiert eine digitale Waage auf einem ESP32.
 * Ein analoger Wägesensor wird über ADC ausgelesen, die Werte werden
 * gefiltert und über Bluetooth als Gewicht in Gramm gesendet.
 * 
 * Hardware:
 * - ESP32 Board (FACD0008)
 * - Wägesensor an GPIO4 (ADC1 Channel 3)
 * - Tare-Button an GPIO42 (gegen GND)
 * 
 * Autor: Mathias Lampert
 * Projekt: Embedded Systems - Digitale Waage
 * ============================================================================
 */

#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include "Bluetooth.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"

/////////////////////////////////////////////////////////////////////////////////////////
// HARDWARE-KONFIGURATION
/////////////////////////////////////////////////////////////////////////////////////////

#define ADC_CHANNEL ADC_CHANNEL_3  // GPIO4
#define ADC_UNIT    ADC_UNIT_1     // ADC Unit 1 (WiFi-kompatibel)
#define TARE_PIN    GPIO_NUM_42    // Tare-Button mit Pull-up
#define TAG "APP"

/////////////////////////////////////////////////////////////////////////////////////////
// KALIBRIERUNGS-KONSTANTEN
/////////////////////////////////////////////////////////////////////////////////////////

/*
 * Lineare Kalibrierung: ADC_ZERO = 0g, ADC_MAX = WEIGHT_MAX
 * Zum Kalibrieren: Leere Waage -> ADC_ZERO, Gewicht auflegen -> ADC_MAX
 */
#define ADC_ZERO 1
#define ADC_MAX  5000
#define WEIGHT_MAX 5000.0  // Gramm

/////////////////////////////////////////////////////////////////////////////////////////
// GLOBALE VARIABLEN
/////////////////////////////////////////////////////////////////////////////////////////

/*
 * Tare Offset: Wird von Messungen abgezogen (Nullpunkt)
 */
static int32_t tare_offset = 0;

/////////////////////////////////////////////////////////////////////////////////////////
// FILTER-EINSTELLUNGEN
/////////////////////////////////////////////////////////////////////////////////////////

/*
 * EMA Filter: EMA_neu = α × Messwert + (1-α) × EMA_alt
 * Alpha: 0.01 = sehr glatt, 0.1 = schneller
 */
#define EMA_ALPHA 0.01

/////////////////////////////////////////////////////////////////////////////////////////
// FUNKTIONEN
/////////////////////////////////////////////////////////////////////////////////////////

/**
 * ADC initialisieren
 * - ADC Unit 1, Channel 3 (GPIO4)
 * - 12-bit Auflösung, 11dB Dämpfung (0-3.3V)
 */
void init_adc(adc_oneshot_unit_handle_t *adc_unit) {
    adc_oneshot_unit_init_cfg_t init_cfg = {.unit_id = ADC_UNIT};
    adc_oneshot_new_unit(&init_cfg, adc_unit);

    adc_oneshot_chan_cfg_t chan_cfg = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_11
    };
    adc_oneshot_config_channel(*adc_unit, ADC_CHANNEL, &chan_cfg);
}

/**
 * GPIO für Tare-Button initialisieren
 * - GPIO42 als Eingang mit Pull-up
 * - Button gegen GND: HIGH = nicht gedrückt, LOW = gedrückt
 */
void init_gpio(void){
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << TARE_PIN,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);
}

/**
 * ADC-Wert in Gramm umrechnen
 * - Tare-Offset abziehen
 * - Lineare Interpolation: Gewicht = ADC × WEIGHT_MAX / ADC_MAX
 */
double adc_to_grams(int adc_val) {
    int adjusted = adc_val - tare_offset;
    if(adjusted < ADC_ZERO) adjusted = 0;
    if(adjusted > ADC_MAX) adjusted = ADC_MAX;
    
    double weight = ((double)adjusted) * WEIGHT_MAX / ((double)ADC_MAX - ADC_ZERO);
    return weight;
}

/////////////////////////////////////////////////////////////////////////////////////////

/**
 * HAUPTPROGRAMM
 * - Initialisierung: Bluetooth, ADC, GPIO
 * - Haupt-Loop: ADC lesen, filtern, senden (50 Hz)
 */
void app_main(void){
    
    // Initialisierung
    bluetooth_init();
    adc_oneshot_unit_handle_t adc1;
    init_adc(&adc1);
    init_gpio();

    int adc_raw = 0;
    
    // EMA Filter mit erstem ADC-Wert initialisieren
    adc_oneshot_read(adc1, ADC_CHANNEL, &adc_raw);
    double ema = (double)adc_raw;

    int log_counter = 0;

    // Haupt-Loop (50 Hz)
    for(;;){
        
        // ADC lesen
        adc_oneshot_read(adc1, ADC_CHANNEL, &adc_raw);

        // EMA-Filter anwenden
        ema = EMA_ALPHA * (double)adc_raw + (1.0 - EMA_ALPHA) * ema;
        int adc_filtered = (int)(ema + 0.5);

        // Tare-Button prüfen
        if(gpio_get_level(TARE_PIN) == 0){
            tare_offset = adc_filtered;
            ESP_LOGI(TAG,"Tare gesetzt: %ld",(long)tare_offset);
        }

        // Gewicht berechnen
        double weight = adc_to_grams(adc_filtered);

        // Logging (nur jeden 10. Wert = 5 Hz)
        log_counter++;
        if(log_counter >= 10){
            ESP_LOGI(TAG,"ADC roh=%d, gefiltert=%d, Gewicht=%.2f g", adc_raw, adc_filtered, weight);
            log_counter = 0;
        }

        // Bluetooth senden (Gewicht × 100 für 2 Dezimalstellen)
        bluetooth_send_int((int32_t)(weight*100));

        // 20ms warten (50 Hz Abtastrate)
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}