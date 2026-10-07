#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_system.h"
#include "stdio.h"

#define ADC_CHANNEL ADC_CHANNEL_0  // ADC-Kanal 0, anpassen nach deinem Pin

void init_adc(adc_oneshot_unit_handle_t *adc_unit) {
    adc_oneshot_unit_init_cfg_t init_cfg = {
        .unit_id = ADC_UNIT_1,
    };
    adc_oneshot_new_unit(&init_cfg, adc_unit);

    adc_oneshot_chan_cfg_t chan_cfg = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,  // 12 Bit
        .atten = ADC_ATTEN_DB_11,          // Spannungsteilung 0-3.6V
    };
    adc_oneshot_config_channel(*adc_unit, ADC_CHANNEL, &chan_cfg);
}

void app_main(void) {
    adc_oneshot_unit_handle_t adc1;
    init_adc(&adc1);

    while (1) {
        int raw;
        adc_oneshot_read(adc1, ADC_CHANNEL, &raw);
        printf("ADC-Rohwert: %d\n", raw);

        vTaskDelay(pdMS_TO_TICKS(1000));  // 1 Sekunde warten
    }
}
