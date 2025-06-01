#include "heart.h"
#include "fft.h"
#include <math.h>
#include <stdlib.h>

// Größe der FFT – muss eine Zweierpotenz sein (z. B. 64, 128, 256)
#define FFT_SIZE 128

// Abtastrate des Sensors in Hz (z. B. 60 Messwerte pro Sekunde)
#define SAMPLE_RATE 60

// Hilfsfunktion zur Bestimmung der Herzfrequenz (BPM) aus dem FFT-Ergebnis
static int find_peak_bpm(complex_t *fft_output, int n) {
    float freq_res = (float)SAMPLE_RATE / n;  // Frequenzauflösung der FFT
    float max_mag = 0;                        // Maximale Amplitude im Frequenzbereich
    int max_idx = 0;                          // Index mit größter Amplitude

    // Interessanter Frequenzbereich: 0.5–3.0 Hz → 30–180 BPM
    int min_bin = (int)(0.5f / freq_res);     
    int max_bin = (int)(3.0f / freq_res);

    // Durchsuche die relevanten Frequenzanteile
    for (int i = min_bin; i <= max_bin; i++) {
        // Betrag (Amplitude) des komplexen FFT-Werts
        float mag = sqrtf(fft_output[i].real * fft_output[i].real + 
                         fft_output[i].imag * fft_output[i].imag);
        // Speichere den höchsten Wert
        if (mag > max_mag) {
            max_mag = mag;
            max_idx = i;
        }
    }

    // Umrechnung von Frequenzindex zu BPM: freq = idx * freq_res; BPM = freq * 60
    return (int)(max_idx * freq_res * 60); // Liefert geschätzte Herzfrequenz
}

// Hauptfunktion zur Berechnung von SpO2 und Pulsfrequenz
void compute_spo2_and_heart_rate(uint32_t *red_buf, uint32_t *ir_buf, int samples, float *spo2, int *bpm) {
    // Variablen zur Mittelwert- und Schwankungsberechnung
    float red_dc = 0, ir_dc = 0;
    float red_ac = 0, ir_ac = 0;

    // Berechne den Gleichanteil (DC) beider Signale
    for (int i = 0; i < samples; i++) {
        red_dc += red_buf[i];
        ir_dc  += ir_buf[i];
    }
    red_dc /= samples;
    ir_dc  /= samples;

    // Berechne den Wechselanteil (AC) durch absolute Abweichung vom Mittelwert
    for (int i = 0; i < samples; i++) {
        red_ac += fabsf((float)red_buf[i] - red_dc);
        ir_ac  += fabsf((float)ir_buf[i] - ir_dc);
    }
    red_ac /= samples;
    ir_ac  /= samples;

    // Prüfe auf gültige Signalqualität (vermeide Berechnung bei schlechten Signalen)
    if (ir_dc < 10000 || red_ac < 20 || ir_ac < 20) {
        *spo2 = 0.0f;
        *bpm = 0;
        return;
    }

    // Verhältnis von AC/DC → R-Wert zur Schätzung von SpO2
    float R = (red_ac / red_dc) / (ir_ac / ir_dc);

    // Näherungsformel zur Berechnung der Sauerstoffsättigung (SpO2)
    *spo2 = 110.0f - 25.0f * R;

    // Wertebereich begrenzen auf 0–100 %
    if (*spo2 > 100.0f) *spo2 = 100.0f;
    if (*spo2 < 0.0f)   *spo2 = 0.0f;

    // Vorbereitung für Herzfrequenzbestimmung mittels FFT
    complex_t fft_input[FFT_SIZE];
    float window[FFT_SIZE];
    hamming_window(window, FFT_SIZE);  // Fensterfunktion reduziert Spektral-Leckage

    // Entferne Mittelwert (DC) und wende Hamming-Fenster an
    float avg = 0;
    for (int i = 0; i < samples; i++) avg += ir_buf[i];
    avg /= samples;

    for (int i = 0; i < FFT_SIZE; i++) {
        float val = (i < samples) ? (ir_buf[i] - avg) : 0; // Zero-Padding ab Sample-Ende
        fft_input[i].real = val * window[i];
        fft_input[i].imag = 0; // Nur Realteil, Imag = 0 für reine Realdaten
    }

    // Berechne die FFT des vorbereiteten Signals
    fft(fft_input, FFT_SIZE);

    // Bestimme die dominierende Frequenzkomponente → Pulsfrequenz in BPM
    *bpm = find_peak_bpm(fft_input, FFT_SIZE);
}
