#pragma once

#include <stdbool.h>

void initI2C(void);
bool shtc3_init(void);
bool shtc3_read(float *temperature, float *humidity);
