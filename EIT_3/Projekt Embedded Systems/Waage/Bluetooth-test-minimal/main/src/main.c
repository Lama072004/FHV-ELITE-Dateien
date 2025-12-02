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

// --- Einstellungen ---
#define ADC_CHANNEL ADC_CHANNEL_2
#define ADC_UNIT    ADC_UNIT_1
#define TARE_PIN    GPIO_NUM_42
#define TAG "APP"

/////////////////////////////////////////////////////////////////////////////////////////

// --- Kalibrierung: 0–1 ADC = 0 g, 3580 ADC = 5000 g ---
#define ADC_ZERO 1
#define ADC_MAX  3580
#define WEIGHT_MAX 5000.0  // Gramm

/////////////////////////////////////////////////////////////////////////////////////////

// --- Tare Offset ---
static int32_t tare_offset = 0;

/////////////////////////////////////////////////////////////////////////////////////////

// --- Kalman Filter Struct ---
typedef struct {
    double x_est;   // estimated value
    double P;       // estimation error
    double Q;       // process noise covariance
    double R;       // measurement noise covariance
} Kalman1D;

// --- Kalman Filter Functions ---
void kalman_init(Kalman1D *k, double init_value, double Q, double R) {
    k->x_est = init_value;
    k->P = 1.0;
    k->Q = Q;
    k->R = R;
}

double kalman_update(Kalman1D *k, double measurement) {
    k->P = k->P + k->Q;
    double K = k->P / (k->P + k->R);
    k->x_est = k->x_est + K * (measurement - k->x_est);
    k->P = (1 - K) * k->P;
    return k->x_est;
}

/////////////////////////////////////////////////////////////////////////////////////////

// --- Kalman Filter Variablen ---
#define KALMAN_Q 0.1     // kleiner → langsame anpassung || größer → schnellere anpassung
#define KALMAN_R 50.0     // größer → Filter misstraut Messung || kleiner → Filter vertraut Messung

/////////////////////////////////////////////////////////////////////////////////////////

// --- ADC Init ---
void init_adc(adc_oneshot_unit_handle_t *adc_unit) {
    adc_oneshot_unit_init_cfg_t init_cfg = {.unit_id = ADC_UNIT};
    adc_oneshot_new_unit(&init_cfg, adc_unit);

    adc_oneshot_chan_cfg_t chan_cfg = {.bitwidth=ADC_BITWIDTH_DEFAULT, .atten=ADC_ATTEN_DB_11};
    adc_oneshot_config_channel(*adc_unit, ADC_CHANNEL, &chan_cfg);
}

/////////////////////////////////////////////////////////////////////////////////////////

// --- GPIO Init ---
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

/////////////////////////////////////////////////////////////////////////////////////////

// --- ADC -> Gramm ---
double adc_to_grams(int adc_val) {
    int adjusted = adc_val - tare_offset;
    if(adjusted < ADC_ZERO) adjusted = 0;
    if(adjusted > ADC_MAX) adjusted = ADC_MAX;
    double weight = ((double)adjusted) * WEIGHT_MAX / ((double)ADC_MAX - ADC_ZERO);
    return weight;
}

/////////////////////////////////////////////////////////////////////////////////////////

void app_main(void){
    bluetooth_init();
    adc_oneshot_unit_handle_t adc1;
    init_adc(&adc1);
    init_gpio();

    int adc_raw = 0;
    Kalman1D kf;

    adc_oneshot_read(adc1, ADC_CHANNEL, &adc_raw);
    kalman_init(&kf, (double)adc_raw, KALMAN_Q, KALMAN_R);

    // EMA Filter Initialisierung
    double ema = (double)adc_raw;
    const double alpha = 0.01; // kleiner = glatter

   int log_counter = 0; // Zähler vor der Schleife

    for(;;){
        adc_oneshot_read(adc1, ADC_CHANNEL, &adc_raw);

        // --- Kalman Filter ---
        double kf_filtered = kalman_update(&kf, (double)adc_raw);

        // --- EMA auf Kalman ---
        ema = alpha * kf_filtered + (1.0 - alpha) * ema;
        int adc_filtered = (int)(ema + 0.5);

        // --- Tare Check ---
        if(gpio_get_level(TARE_PIN) == 0){
            tare_offset = adc_filtered;
            ESP_LOGI(TAG,"Tare gesetzt: %ld",(long)tare_offset);
        }

        // --- Gewicht berechnen ---
        double weight = adc_to_grams(adc_filtered);

        // --- nur jeden 10. Wert loggen ---
        log_counter++;
        if(log_counter >= 10){
            ESP_LOGI(TAG,"ADC roh=%d, gefiltert=%d, Gewicht=%.2f g", adc_raw, adc_filtered, weight);
            log_counter = 0;
        }

        // --- Bluetooth senden ---
        bluetooth_send_int((int32_t)(weight*100)); // 2 Dezimalstellen

        vTaskDelay(pdMS_TO_TICKS(8)); // 125 Hz
    }
}


// to do: Kalman Filter entfernen und besseren filter implementieren