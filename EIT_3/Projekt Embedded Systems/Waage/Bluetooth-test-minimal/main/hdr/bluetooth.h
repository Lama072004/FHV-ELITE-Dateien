#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

// --- Methoden ---
void bluetooth_init(void);
bool bluetooth_receive_int(int32_t *out_value, uint32_t timeout_ms);
esp_err_t bluetooth_send_int(int32_t value);

#endif // BLUETOOTH_H
