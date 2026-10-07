#ifndef HEART_RATE_H
#define HEART_RATE_H

#include <stdint.h>

int hillclimb_peak_detection(uint32_t *data, int size, int min_distance, float threshold_ratio);
void compute_spo2_and_heart_rate(uint32_t *red_buf, uint32_t *ir_buf, int samples, float *spo2, int *bpm);

#endif // HEART_RATE_H
