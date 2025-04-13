#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"

#define LED_GPIO         8
#define NUM_LEDS         25
#define DELAY_MS         150
#define CENTER_BRIGHT    255
#define SIDE_BRIGHT      60

static const char *TAG = "KITT_EFFECT";

static led_strip_handle_t led_strip;
static int direction = 1;
static int position = 1;
static int hue = 0;

static void setup_led_strip() {
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
    ESP_LOGI(TAG, "LED-Strip mit %d LEDs initialisiert", NUM_LEDS);
}

static void set_led_hsv(int led_num, int hue, int brightness) {
    uint8_t r = 0, g = 0, b = 0;

    hue = hue % 360;

    if (hue < 60) {
        r = 255;
        g = hue * 4.25;
    } else if (hue < 120) {
        r = 255 - (hue - 60) * 4.25;
        g = 255;
    } else if (hue < 180) {
        g = 255;
        b = (hue - 120) * 4.25;
    } else if (hue < 240) {
        g = 255 - (hue - 180) * 4.25;
        b = 255;
    } else if (hue < 300) {
        r = (hue - 240) * 4.25;
        b = 255;
    } else {
        r = 255;
        b = 255 - (hue - 300) * 4.25;
    }

    r = (r * brightness) / 255;
    g = (g * brightness) / 255;
    b = (b * brightness) / 255;

    led_strip_set_pixel(led_strip, led_num, r, g, b);
}

static void update_kitt_effect() {
    led_strip_clear(led_strip);

    for (int offset = -1; offset <= 1; offset++) {
        int led_pos = position + offset;

        if (led_pos >= 0 && led_pos < NUM_LEDS) {
            uint8_t brightness = (offset == 0) ? CENTER_BRIGHT : SIDE_BRIGHT;
            set_led_hsv(led_pos, hue, brightness);
        }
    }

    led_strip_refresh(led_strip);
}

void app_main() {
    setup_led_strip();

    while (1) {
        update_kitt_effect();
        position += direction;

        if (position <= 1 || position >= NUM_LEDS - 2) {
            direction *= -1;
        }

        hue = (hue + 10) % 360;

        vTaskDelay(DELAY_MS / portTICK_PERIOD_MS);
    }
}
