#include "heart.h"
#include <math.h>

int hillclimb_peak_detection(uint32_t *data, int size, int min_distance, float threshold_ratio) {
    int peaks = 0;
    int last_peak = -min_distance;

    float max_val = 0;
    for (int i = 0; i < size; i++) {
        if (data[i] > max_val) max_val = data[i];
    }

    float threshold = max_val * threshold_ratio;

    for (int i = 1; i < size - 1; i++) {
        if (data[i] > threshold &&
            data[i] > data[i-1] &&
            data[i] > data[i+1] &&
            (i - last_peak) >= min_distance) {

            peaks++;
            last_peak = i;
        }
    }
    return peaks;
}

void compute_spo2_and_heart_rate(uint32_t *red_buf, uint32_t *ir_buf, int samples, float *spo2, int *bpm) {
    float red_dc = 0, ir_dc = 0;
    float red_ac = 0, ir_ac = 0;

    for (int i = 0; i < samples; i++) {
        red_dc += red_buf[i];
        ir_dc  += ir_buf[i];
    }
    red_dc /= samples;
    ir_dc  /= samples;

    for (int i = 0; i < samples; i++) {
        red_ac += fabsf((float)red_buf[i] - red_dc);
        ir_ac  += fabsf((float)ir_buf[i] - ir_dc);
    }
    red_ac /= samples;
    ir_ac  /= samples;

    if (ir_dc < 10000 || red_ac < 20 || ir_ac < 20) {
        *spo2 = 0.0f;
        *bpm = 0;
        return;
    }

    float R = (red_ac / red_dc) / (ir_ac / ir_dc);
    *spo2 = 110.0f - 25.0f * R;
    if (*spo2 > 100.0f) *spo2 = 100.0f;
    if (*spo2 < 0.0f)   *spo2 = 0.0f;

    int peaks = hillclimb_peak_detection(ir_buf, samples, 6, 0.6f); // Abstand = 6 Samples, Threshold = 60%
    *bpm = (int)(peaks * 60 / 10); // Annahme: 10 Sekunden Daten
}
