#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

// --- Methoden ---
void bluetooth_init(void);
void bluetooth_send_int(int32_t value);  // void statt esp_err_t

#endif // BLUETOOTH_H
