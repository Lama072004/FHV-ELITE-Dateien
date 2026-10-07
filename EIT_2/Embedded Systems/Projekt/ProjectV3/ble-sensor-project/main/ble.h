#ifndef BLE_H
#define BLE_H

#include <stdint.h>

#define DEVICE_NAME "PulseOxy"
#define UUID_HEART_RATE_SERVICE 0x180D
#define UUID_HEART_RATE_MEASUREMENT 0x2A37

void ble_init(void);
void ble_notify_values(uint8_t pulse, float spo2);

#endif // BLE_H