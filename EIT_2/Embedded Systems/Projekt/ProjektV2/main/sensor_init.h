#ifndef SENSOR_INIT_H
#define SENSOR_INIT_H

#include <stdint.h>

void initI2C();
void max30102_init();
void max30102_read_fifo(uint32_t *red, uint32_t *ir);

#endif // SENSOR_INIT_H
