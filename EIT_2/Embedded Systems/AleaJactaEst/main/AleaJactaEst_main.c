#include <stdio.h>
#include <inttypes.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"

#define RNG_BASE 0x60026000
#define RNG_DATA_REG_OFFS 0xB0

#define BUTTON_ROLL  GPIO_NUM_2
#define BUTTON_CHEAT GPIO_NUM_9

#define MIN_VALUE 1
#define MAX_VALUE 6
#define DEBOUNCE_MS 500

#define LED_GPIO         8
#define NUM_LEDS         25

static led_strip_handle_t led_strip;
static bool cheat_mode = false;

uint32_t read_trng(void) {
    return *(volatile uint32_t *)(RNG_BASE + RNG_DATA_REG_OFFS);
}

uint32_t get_rand_in_range(uint32_t min, uint32_t max) {
    return min + (read_trng() % (max - min + 1));
}

void init_button(gpio_num_t gpio) {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << gpio),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);
}

bool check_button_pressed(gpio_num_t gpio) {
    static uint32_t last_pressed_time[40] = {0};
    if (gpio_get_level(gpio) == 0) {
        uint32_t now = xTaskGetTickCount() * portTICK_PERIOD_MS;
        if (now - last_pressed_time[gpio] > DEBOUNCE_MS) {
            last_pressed_time[gpio] = now;
            return true;
        }
    }
    return false;
}

void led_number(int number) {
    led_strip_clear(led_strip);  // Alle LEDs ausschalten

    switch (number) {
    case 1:
        led_strip_set_pixel(led_strip, 3, 255, 0, 0);
        led_strip_set_pixel(led_strip, 8, 255, 0, 0);
        led_strip_set_pixel(led_strip, 13, 255, 0, 0);
        led_strip_set_pixel(led_strip, 18, 255, 0, 0);
        led_strip_set_pixel(led_strip, 23, 255, 0, 0);
        break;
    case 2:
        // Segment A (oben horizontal)
        led_strip_set_pixel(led_strip, 1, 255, 0, 0);
        led_strip_set_pixel(led_strip, 2, 255, 0, 0);
        led_strip_set_pixel(led_strip, 3, 255, 0, 0);

        // Segment B (rechts oben vertikal)
        led_strip_set_pixel(led_strip, 4, 255, 0, 0);
        led_strip_set_pixel(led_strip, 9, 255, 0, 0);
        led_strip_set_pixel(led_strip, 14, 255, 0, 0);

        // Segment G (Mitte horizontal)
        led_strip_set_pixel(led_strip, 8, 255, 0, 0);
        led_strip_set_pixel(led_strip, 9, 255, 0, 0);
        led_strip_set_pixel(led_strip, 10, 255, 0, 0);

        // Segment E (links unten vertikal)
        led_strip_set_pixel(led_strip, 15, 255, 0, 0);
        led_strip_set_pixel(led_strip, 16, 255, 0, 0);
        led_strip_set_pixel(led_strip, 17, 255, 0, 0);

        // Segment D (unten horizontal)
        led_strip_set_pixel(led_strip, 14, 255, 0, 0);
        led_strip_set_pixel(led_strip, 15, 255, 0, 0);
        led_strip_set_pixel(led_strip, 16, 255, 0, 0);

        // Segment F (links oben vertikal)
        led_strip_set_pixel(led_strip, 3, 255, 0, 0);
        led_strip_set_pixel(led_strip, 8, 255, 0, 0);
        led_strip_set_pixel(led_strip, 13, 255, 0, 0);

        // Segment C (rechts unten vertikal)
        led_strip_set_pixel(led_strip, 22, 255, 0, 0);
        led_strip_set_pixel(led_strip, 21, 255, 0, 0);
        led_strip_set_pixel(led_strip, 20, 255, 0, 0);
        break;
    case 3:
        // Segment A (oben horizontal)
        led_strip_set_pixel(led_strip, 1, 255, 0, 0);
        led_strip_set_pixel(led_strip, 2, 255, 0, 0);
        led_strip_set_pixel(led_strip, 3, 255, 0, 0);

        // Segment B (rechts oben vertikal)
        led_strip_set_pixel(led_strip, 4, 255, 0, 0);
        led_strip_set_pixel(led_strip, 9, 255, 0, 0);
        led_strip_set_pixel(led_strip, 14, 255, 0, 0);

        // Segment G (Mitte horizontal)
        led_strip_set_pixel(led_strip, 8, 255, 0, 0);
        led_strip_set_pixel(led_strip, 9, 255, 0, 0);
        led_strip_set_pixel(led_strip, 10, 255, 0, 0);

        // Segment E (links unten vertikal)
        led_strip_set_pixel(led_strip, 15, 255, 0, 0);
        led_strip_set_pixel(led_strip, 16, 255, 0, 0);
        led_strip_set_pixel(led_strip, 17, 255, 0, 0);

        // Segment D (unten horizontal)
        led_strip_set_pixel(led_strip, 14, 255, 0, 0);
        led_strip_set_pixel(led_strip, 15, 255, 0, 0);
        led_strip_set_pixel(led_strip, 16, 255, 0, 0);

        // Segment F (links oben vertikal)
        led_strip_set_pixel(led_strip, 3, 255, 0, 0);
        led_strip_set_pixel(led_strip, 8, 255, 0, 0);
        led_strip_set_pixel(led_strip, 13, 255, 0, 0);

        // Segment C (rechts unten vertikal)
        led_strip_set_pixel(led_strip, 22, 255, 0, 0);
        led_strip_set_pixel(led_strip, 21, 255, 0, 0);
        led_strip_set_pixel(led_strip, 20, 255, 0, 0);
        break;
    case 4:
        // Segment A (oben horizontal)
        led_strip_set_pixel(led_strip, 1, 255, 0, 0);
        led_strip_set_pixel(led_strip, 2, 255, 0, 0);
        led_strip_set_pixel(led_strip, 3, 255, 0, 0);

        // Segment B (rechts oben vertikal)
        led_strip_set_pixel(led_strip, 4, 255, 0, 0);
        led_strip_set_pixel(led_strip, 9, 255, 0, 0);
        led_strip_set_pixel(led_strip, 14, 255, 0, 0);

        // Segment G (Mitte horizontal)
        led_strip_set_pixel(led_strip, 8, 255, 0, 0);
        led_strip_set_pixel(led_strip, 9, 255, 0, 0);
        led_strip_set_pixel(led_strip, 10, 255, 0, 0);

        // Segment E (links unten vertikal)
        led_strip_set_pixel(led_strip, 15, 255, 0, 0);
        led_strip_set_pixel(led_strip, 16, 255, 0, 0);
        led_strip_set_pixel(led_strip, 17, 255, 0, 0);

        // Segment D (unten horizontal)
        led_strip_set_pixel(led_strip, 14, 255, 0, 0);
        led_strip_set_pixel(led_strip, 15, 255, 0, 0);
        led_strip_set_pixel(led_strip, 16, 255, 0, 0);

        // Segment F (links oben vertikal)
        led_strip_set_pixel(led_strip, 3, 255, 0, 0);
        led_strip_set_pixel(led_strip, 8, 255, 0, 0);
        led_strip_set_pixel(led_strip, 13, 255, 0, 0);

        // Segment C (rechts unten vertikal)
        led_strip_set_pixel(led_strip, 22, 255, 0, 0);
        led_strip_set_pixel(led_strip, 21, 255, 0, 0);
        led_strip_set_pixel(led_strip, 20, 255, 0, 0);
        break;
    case 5:
        // Segment A (oben horizontal)
        led_strip_set_pixel(led_strip, 1, 255, 0, 0);
        led_strip_set_pixel(led_strip, 2, 255, 0, 0);
        led_strip_set_pixel(led_strip, 3, 255, 0, 0);

        // Segment B (rechts oben vertikal)
        led_strip_set_pixel(led_strip, 4, 255, 0, 0);
        led_strip_set_pixel(led_strip, 9, 255, 0, 0);
        led_strip_set_pixel(led_strip, 14, 255, 0, 0);

        // Segment G (Mitte horizontal)
        led_strip_set_pixel(led_strip, 8, 255, 0, 0);
        led_strip_set_pixel(led_strip, 9, 255, 0, 0);
        led_strip_set_pixel(led_strip, 10, 255, 0, 0);

        // Segment E (links unten vertikal)
        led_strip_set_pixel(led_strip, 15, 255, 0, 0);
        led_strip_set_pixel(led_strip, 16, 255, 0, 0);
        led_strip_set_pixel(led_strip, 17, 255, 0, 0);

        // Segment D (unten horizontal)
        led_strip_set_pixel(led_strip, 14, 255, 0, 0);
        led_strip_set_pixel(led_strip, 15, 255, 0, 0);
        led_strip_set_pixel(led_strip, 16, 255, 0, 0);

        // Segment F (links oben vertikal)
        led_strip_set_pixel(led_strip, 3, 255, 0, 0);
        led_strip_set_pixel(led_strip, 8, 255, 0, 0);
        led_strip_set_pixel(led_strip, 13, 255, 0, 0);

        // Segment C (rechts unten vertikal)
        led_strip_set_pixel(led_strip, 22, 255, 0, 0);
        led_strip_set_pixel(led_strip, 21, 255, 0, 0);
        led_strip_set_pixel(led_strip, 20, 255, 0, 0);
        break;
    case 6:
        // Segment A (oben horizontal)
        led_strip_set_pixel(led_strip, 1, 255, 0, 0);
        led_strip_set_pixel(led_strip, 2, 255, 0, 0);
        led_strip_set_pixel(led_strip, 3, 255, 0, 0);

        // Segment B (rechts oben vertikal)
        led_strip_set_pixel(led_strip, 4, 255, 0, 0);
        led_strip_set_pixel(led_strip, 9, 255, 0, 0);
        led_strip_set_pixel(led_strip, 14, 255, 0, 0);

        // Segment G (Mitte horizontal)
        led_strip_set_pixel(led_strip, 8, 255, 0, 0);
        led_strip_set_pixel(led_strip, 9, 255, 0, 0);
        led_strip_set_pixel(led_strip, 10, 255, 0, 0);

        // Segment E (links unten vertikal)
        led_strip_set_pixel(led_strip, 15, 255, 0, 0);
        led_strip_set_pixel(led_strip, 16, 255, 0, 0);
        led_strip_set_pixel(led_strip, 17, 255, 0, 0);

        // Segment D (unten horizontal)
        led_strip_set_pixel(led_strip, 14, 255, 0, 0);
        led_strip_set_pixel(led_strip, 15, 255, 0, 0);
        led_strip_set_pixel(led_strip, 16, 255, 0, 0);

        // Segment F (links oben vertikal)
        led_strip_set_pixel(led_strip, 3, 255, 0, 0);
        led_strip_set_pixel(led_strip, 8, 255, 0, 0);
        led_strip_set_pixel(led_strip, 13, 255, 0, 0);

        // Segment C (rechts unten vertikal)
        led_strip_set_pixel(led_strip, 22, 255, 0, 0);
       

    led_strip_refresh(led_strip);
}

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
}

void app_main(void) {
    setup_led_strip();
    init_button(BUTTON_ROLL);
    init_button(BUTTON_CHEAT);

    printf("ESP-Würfel bereit! SW3: Würfeln | SW2: Cheat\n");

    while (1) {
        if (check_button_pressed(BUTTON_CHEAT)) {
            cheat_mode = true;
            printf("Cheat-Modus aktiviert nächste Zahl ist eine 6!\n");
        }

        if (check_button_pressed(BUTTON_ROLL)) {
            int result;

            if (cheat_mode) {
                result = 6;
                cheat_mode = false;
            } else {
                result = get_rand_in_range(MIN_VALUE, MAX_VALUE);
            }

            led_number(result);

            printf("Gewürfelt: %d\n", result);
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
