#ifndef SENSOR_READ_WRITE_H
#define SENSOR_READ_WRITE_H

#include <stdint.h>
#include <stddef.h>

void max30102_read_fifo(uint32_t *red, uint32_t *ir);

int read_register(uint8_t reg, uint8_t *buf, size_t len);

#endif
