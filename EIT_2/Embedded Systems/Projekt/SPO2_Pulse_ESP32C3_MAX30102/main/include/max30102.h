#pragma once
#include <stdint.h>
#include "esp_err.h"

#define I2C_MASTER_SCL_IO          0
#define I2C_MASTER_SDA_IO          1
#define I2C_MASTER_NUM             I2C_NUM_0
#define I2C_MASTER_FREQ_HZ         400000

void max30102_init(void);
void max30102_read_fifo(uint32_t *red, uint32_t *ir);
