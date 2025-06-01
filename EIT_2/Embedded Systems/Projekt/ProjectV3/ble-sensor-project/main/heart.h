#ifndef HEART_H
#define HEART_H

#include <stdint.h>

void heart_init(void);
void heart_read_data(uint8_t *pulse, float *spo2);

#endif // HEART_H