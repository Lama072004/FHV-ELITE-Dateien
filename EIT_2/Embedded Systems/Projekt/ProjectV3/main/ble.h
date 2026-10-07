#ifndef BLE_H
#define BLE_H

#include <stdint.h>

// Initialisiert den BLE-Server (inkl. Advertising, Services etc.)
void ble_init(void);

// Startet die Benachrichtigungen (Notify) für die Messwerte
void ble_start_notify_task(void);

// Sende die aktuellen Messwerte (z.B. Puls, SpO2) per BLE-Notify (optional, falls du manuelles Senden möchtest)
void ble_notify_values(uint8_t pulse, float spo2);

void ble_send_string(const char *str);

#endif
