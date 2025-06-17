#include "heart.h"
#include "fft.h"
#include <math.h>
#include <stdlib.h>

// FFT-Größe: Anzahl der Samples für Frequenzanalyse (muss 2^n sein)
#define FFT_SIZE 128

// Sensor-Abtastrate (Anzahl Messungen pro Sekunde)
#define SAMPLE_RATE 60

// Interne Hilfsfunktion: Findet die dominante Frequenz in der FFT (entspricht Herzfrequenz)
static int find_peak_bpm(complex_t *fft_output, int n) {
    float freq_res = (float)SAMPLE_RATE / n;  // Frequenzauflösung (Hz pro Bin)
    float max_mag = 0;                        // höchste Amplitude
    int max_idx = 0;                          // Index der dominanten Frequenz

    // Nur realistische Herzfrequenzbereiche betrachten: 30–180 BPM (≈ 0.5–3 Hz)
    int min_bin = (int)(0.5f / freq_res);
    int max_bin = (int)(3.0f / freq_res);

    for (int i = min_bin; i <= max_bin; i++) {
        // Betrag des komplexen FFT-Werts
        float mag = sqrtf(fft_output[i].real * fft_output[i].real +
                          fft_output[i].imag * fft_output[i].imag);

        if (mag > max_mag) {
            max_mag = mag;
            max_idx = i;
        }
    }

    // Frequenz → BPM: f * 60
    return (int)(max_idx * freq_res * 60);
}

// Hauptfunktion: Berechnet SpO2 und Herzfrequenz (bpm) aus den Sensorwerten
void compute_spo2_and_heart_rate(uint32_t *red_buf, uint32_t *ir_buf, int samples, float *spo2, int *bpm) {
    float red_dc = 0, ir_dc = 0;  // Mittelwerte (DC-Anteil)
    float red_ac = 0, ir_ac = 0;  // Schwankungen (AC-Anteil)

    // Mittelwert (DC) der beiden Kanäle berechnen
    for (int i = 0; i < samples; i++) {
        red_dc += red_buf[i];
        ir_dc  += ir_buf[i];
    }
    red_dc /= samples;
    ir_dc  /= samples;

    // Schwankungen (AC) als mittlere Abweichung vom DC berechnen
    for (int i = 0; i < samples; i++) {
        red_ac += fabsf((float)red_buf[i] - red_dc);
        ir_ac  += fabsf((float)ir_buf[i] - ir_dc);
    }
    red_ac /= samples;
    ir_ac  /= samples;

    // Prüfen, ob Signalqualität ausreichend ist
    if (ir_dc < 10000 || red_ac < 20 || ir_ac < 20) {
        *spo2 = 0.0f;
        *bpm = 0;
        return;
    }

    // Verhältnis der Normierten Amplituden: R-Wert zur SpO2-Schätzung
    float R = (red_ac / red_dc) / (ir_ac / ir_dc);

    // Näherungsformel zur Schätzung der Sauerstoffsättigung
    *spo2 = 110.0f - 25.0f * R;

    // Begrenzen auf physikalisch sinnvollen Bereich (0–100 %)
    if (*spo2 > 100.0f) *spo2 = 100.0f;
    if (*spo2 < 0.0f)   *spo2 = 0.0f;

    // Vorbereitung für FFT zur Pulserkennung
    complex_t fft_input[FFT_SIZE];
    float window[FFT_SIZE];
    hamming_window(window, FFT_SIZE);  // Fenster glättet das Signal

    // Mittelwert entfernen (DC-Abzug) + Fenster anwenden
    float avg = 0;
    for (int i = 0; i < samples; i++) avg += ir_buf[i];
    avg /= samples;

    for (int i = 0; i < FFT_SIZE; i++) {
        float val = (i < samples) ? (ir_buf[i] - avg) : 0.0f;  // Zero-Padding ab Ende
        fft_input[i].real = val * window[i];  // Fensterung des Realteils
        fft_input[i].imag = 0.0f;             // Imaginärteil = 0
    }

    // FFT berechnen
    fft(fft_input, FFT_SIZE);

    // Herzfrequenz aus dominanter Frequenzkomponente ermitteln
    *bpm = find_peak_bpm(fft_input, FFT_SIZE);
}
