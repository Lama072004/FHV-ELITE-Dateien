#pragma once
#include <stdbool.h>
#include "driver/gpio.h"

typedef void (*button_callback_t)(void*);

typedef struct {
    gpio_num_t gpio_num;
    bool pressed;
    uint32_t last_press_time;
    button_callback_t callback;
    void* callback_arg;
} button_t;

void button_init(button_t* btn, gpio_num_t gpio, button_callback_t callback, void* arg);
bool button_check_pressed(button_t* btn);

// Initialisierungs-Makro (nur bei Deklaration verwenden!)
#define BUTTON_INIT(gpio, cb, arg) \
    { .gpio_num = gpio, .pressed = false, .last_press_time = 0, .callback = cb, .callback_arg = arg }
