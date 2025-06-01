#include "fft.h"

// Hamming-Fenster vorbereiten (zur Glättung des Spektrums, um Leakage zu reduzieren)
void hamming_window(float *window, int n) {
    for (int i = 0; i < n; i++) {
        // Hamming-Funktion: w[i] = 0.54 - 0.46 * cos(2πi / (n - 1))
        window[i] = 0.54f - 0.46f * cosf(2 * M_PI * i / (n - 1));
    }
}

// Hilfsfunktion: Bit-Reversal (z. B. 0110 -> 0110, 0011 -> 1100 bei 4 Bit)
static unsigned int reverse_bits(unsigned int x, int bits) {
    unsigned int result = 0;
    for (int i = 0; i < bits; i++) {
        result = (result << 1) | (x & 1);  // niedrigstes Bit übernehmen
        x >>= 1;                           // nächste Stelle holen
    }
    return result;
}

// Cooley-Tukey Radix-2 FFT (In-place, ohne komplexe Libs)
void fft(complex_t *buf, int n) {
    // Bit-Reversal-Permutation der Eingangsdaten (Index-Umsortierung)
    int bits = (int)log2(n);  // Anzahl Bits zur Darstellung von n Positionen
    for (int i = 0; i < n; i++) {
        int j = reverse_bits(i, bits);  // Zielindex durch Bitumkehr
        if (j > i) {  // Nur vertauschen, wenn j > i (sonst doppelt)
            complex_t tmp = buf[i];
            buf[i] = buf[j];
            buf[j] = tmp;
        }
    }

    // Haupt-FFT-Schleife: Blockweise Butterfly-Verarbeitung
    for (int len = 2; len <= n; len <<= 1) {
        float angle = -2 * M_PI / len;  // Winkelinkrement pro Schritt
        complex_t wlen = {cosf(angle), sinf(angle)};  // Drehfaktor für aktuelle Länge

        for (int i = 0; i < n; i += len) {
            complex_t w = {1.0f, 0.0f};  // Startwert für Rotation (Einheitskomplexzahl)
            for (int j = 0; j < len / 2; j++) {
                complex_t even = buf[i + j];               // Gerade Position
                complex_t odd  = buf[i + j + len / 2];     // Ungerade Position

                // Butterfly-Berechnung
                float treal = w.real * odd.real - w.imag * odd.imag;
                float timag = w.real * odd.imag + w.imag * odd.real;

                buf[i + j].real         = even.real + treal;
                buf[i + j].imag         = even.imag + timag;
                buf[i + j + len / 2].real = even.real - treal;
                buf[i + j + len / 2].imag = even.imag - timag;

                // Winkelrotation: w = w * wlen
                float w_real_new = w.real * wlen.real - w.imag * wlen.imag;
                w.imag = w.real * wlen.imag + w.imag * wlen.real;
                w.real = w_real_new;
            }
        }
    }
}
