#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_random.h"
#include "button.h"

// Forward declarations
static void roll_button_callback(void* arg);
static void cheat_button_callback(void* arg);

static bool cheat_mode = false;

// Initialisierung über Makro
static button_t roll_btn = BUTTON_INIT(GPIO_NUM_2, roll_button_callback, NULL);
static button_t cheat_btn = BUTTON_INIT(GPIO_NUM_9, cheat_button_callback, NULL);

static void roll_button_callback(void* arg) {
    int result = cheat_mode ? 6 : (esp_random() % 6) + 1;
    printf("Gewürfelt: %d\n", result);
    cheat_mode = false;
}

static void cheat_button_callback(void* arg) {
    cheat_mode = true;
    printf("Cheat-Modus aktiviert!\n");
}

void app_main(void) {
    // Buttons hardwareseitig initialisieren
    button_init(&roll_btn, roll_btn.gpio_num, roll_btn.callback, roll_btn.callback_arg);
    button_init(&cheat_btn, cheat_btn.gpio_num, cheat_btn.callback, cheat_btn.callback_arg);

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
