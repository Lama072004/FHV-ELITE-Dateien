#include "button.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define DEBOUNCE_MS 500

static bool isr_service_installed = false;

void button_isr_handler(void* arg) {
    button_t* btn = (button_t*)arg;
    uint32_t now = xTaskGetTickCount() * portTICK_PERIOD_MS;

    if (now - btn->last_press_time > DEBOUNCE_MS) {
        btn->pressed = true;
        btn->last_press_time = now;

        if (btn->callback) {
            btn->callback(btn->callback_arg);
        }
    }
}

void button_init(button_t* btn, gpio_num_t gpio, button_callback_t callback, void* arg) {
    btn->gpio_num = gpio;
    btn->pressed = false;
    btn->last_press_time = 0;
    btn->callback = callback;
    btn->callback_arg = arg;

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << gpio),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE
    };
    gpio_config(&io_conf);

    if (!isr_service_installed) {
        gpio_install_isr_service(0);
        isr_service_installed = true;
    }

    gpio_isr_handler_add(gpio, button_isr_handler, (void*)btn);
}

bool button_check_pressed(button_t* btn) {
    if (btn->pressed) {
        btn->pressed = false;
        return true;
    }
    return false;
}
