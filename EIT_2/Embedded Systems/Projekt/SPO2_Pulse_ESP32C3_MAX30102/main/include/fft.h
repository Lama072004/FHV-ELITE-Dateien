#ifndef FFT_H
#define FFT_H

#include <stdint.h>
#include <math.h>

typedef struct {
    float real;
    float imag;
} complex_t;

void fft(complex_t *buf, int n);
void hamming_window(float *window, int n);

#endif