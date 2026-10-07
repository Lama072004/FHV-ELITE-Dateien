#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "esp_log.h"

#define LED_GPIO 8
#define NUM_LEDS 25
#define BPM 80
#define FRAME_DELAY_MS 60
#define PULSE_PERIOD_MS (60000.0 / BPM)   // ~750 ms

static led_strip_handle_t led_strip;

void setup_led_strip() {
    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_GPIO,
        .max_leds = NUM_LEDS,
    };

    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000,
        .flags.with_dma = false,
    };

    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    led_strip_clear(led_strip);
    led_strip_refresh(led_strip);
}

void draw_led(float brightness) {
    int red = (int)(brightness * 255); // Umrechnung 0..1 → 0..255

    led_strip_clear(led_strip);

    // Deine definierten Pixel (Linie)
    led_strip_set_pixel(led_strip, 10, red, 0, 0);
    led_strip_set_pixel(led_strip, 11,  red, 0, 0);
    led_strip_set_pixel(led_strip, 12, red, 0, 0);
    led_strip_set_pixel(led_strip, 13,  red, 0, 0);
    led_strip_set_pixel(led_strip, 14,  red, 0, 0);
    led_strip_refresh(led_strip);
}

void app_main(void) {
    setup_led_strip();

    float time_ms = 0;

    while (1) {
        // Sanfte Pulswelle (80 BPM → ~0,75 s pro Zyklus)
        float phase = (2 * M_PI * time_ms) / PULSE_PERIOD_MS;
        float brightness = 0.5f + 0.5f * sinf(phase);  // 0..1 Helligkeit
        brightness = powf(brightness, 1.5f);           // weichere Helligkeitskurve

        draw_led(brightness);

        vTaskDelay(pdMS_TO_TICKS(FRAME_DELAY_MS));
        time_ms += FRAME_DELAY_MS;

        if (time_ms >= PULSE_PERIOD_MS) {
            time_ms = 0;
        }
    }
}
