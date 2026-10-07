#ifndef CONTROLS_H
#define CONTROLS_H

#include <stdbool.h>
#include "driver/gpio.h"
#include "Globals.h"
// TickType_t is defined in FreeRTOS headers
#include "freertos/FreeRTOS.h"

void init_controls(void);
// Non-blocking: try to get a button event from ISR queue. Returns true if event available.
// out_gpio will be set to the gpio number of the pressed button.
bool controls_get_event(gpio_num_t *out_gpio);

// Blocking wait for event with timeout in ticks
bool controls_wait_event(gpio_num_t *out_gpio, TickType_t ticks_to_wait);

// Deprecated: polling helper (kept for compatibility)
bool check_button_pressed(gpio_num_t gpio);

// Returns true if all defined buttons are currently pressed (active low)
bool controls_all_buttons_pressed(void);

// Disable ISR for all buttons (prevents new events from being queued)
void controls_disable_isr(void);

// Enable ISR for all buttons
void controls_enable_isr(void);

#endif // CONTROLS_H
