#include <stdio.h>
#include <stdlib.h>     
#include <time.h>       
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "led_strip.h"

#define LED_GPIO 8
#define WIDTH 5
#define HEIGHT 5
#define NUM_LEDS (WIDTH * HEIGHT)

// Anzahl der Bälle
#define NUM_BALLS 5  

static led_strip_handle_t led_strip;

// Hilfsfunktion: 2D-Koordinaten in LED-Index umwandeln (Zeile-major)
int xy_to_index(int x, int y) {
    return y * WIDTH + x;
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
    led_strip_refresh(led_strip);

    // Zufall initialisieren
    srand(xTaskGetTickCount());
}

// Hilfsfunktion: zufällige Farbe generieren
void random_color(int *r, int *g, int *b) {
    *r = rand() % 150;
    *g = rand() % 150;
    *b = rand() % 150;
}

// Hilfsfunktion: zufällige Richtung wählen (-1,0,+1), nicht 0,0
void random_direction(int *dx, int *dy) {
    do {
        *dx = (rand() % 3) - 1;
        *dy = (rand() % 3) - 1;
    } while (*dx == 0 && *dy == 0);
}

void app_main(void) {
    setup_led_strip();

    // Arrays für Bälle
    int x[NUM_BALLS], y[NUM_BALLS];
    int dx[NUM_BALLS], dy[NUM_BALLS];
    int r[NUM_BALLS], g[NUM_BALLS], b[NUM_BALLS];

    // Bälle initialisieren
    for (int i = 0; i < NUM_BALLS; i++) {
        x[i] = rand() % WIDTH;
        y[i] = rand() % HEIGHT;
        random_direction(&dx[i], &dy[i]);
        random_color(&r[i], &g[i], &b[i]);
    }

    while (1) {
        led_strip_clear(led_strip);

        for (int i = 0; i < NUM_BALLS; i++) {
            // Ballposition anzeigen
            led_strip_set_pixel(led_strip, xy_to_index(x[i], y[i]), r[i], g[i], b[i]);

            // Nächste Position berechnen
            int nx = x[i] + dx[i];
            int ny = y[i] + dy[i];

            int bounced = 0;
            if (nx < 0 || nx >= WIDTH) {
                dx[i] = -dx[i];
                nx = x[i] + dx[i];
                bounced = 1;
            }
            if (ny < 0 || ny >= HEIGHT) {
                dy[i] = -dy[i];
                ny = y[i] + dy[i];
                bounced = 1;
            }

            // Farbe bei Bounce ändern
            if (bounced) {
                random_color(&r[i], &g[i], &b[i]);
            }

            // Zufällige Richtungsänderung (≈10%)
            if (rand() % 10 == 0) {
                random_direction(&dx[i], &dy[i]);
            }

            // Position aktualisieren
            x[i] = nx;
            y[i] = ny;
        }

        led_strip_refresh(led_strip);
        vTaskDelay(pdMS_TO_TICKS(300));
    }
}
