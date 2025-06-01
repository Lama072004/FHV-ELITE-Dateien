#pragma once
#include <stdint.h>

void compute_spo2_and_heart_rate(uint32_t *red_buf, uint32_t *ir_buf, int samples, float *spo2, int *bpm);
