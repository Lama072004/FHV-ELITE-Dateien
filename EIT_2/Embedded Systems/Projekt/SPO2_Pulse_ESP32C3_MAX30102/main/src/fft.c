#include "fft.h"

// Hamming-Fenster vorbereiten
// → Glättet das Signal vor der FFT, um Spektrum-Leakage zu minimieren
void hamming_window(float *window, int n) {
    for (int i = 0; i < n; i++) {
        // Hamming-Formel: w[i] = 0.54 - 0.46 * cos(2πi / (n - 1))
        window[i] = 0.54f - 0.46f * cosf(2 * M_PI * i / (n - 1));
    }
}

// Bit-Reversal (Hilfsfunktion)
// → Vertauscht die Bit-Reihenfolge einer Ganzzahl mit fester Bitlänge
// Beispiel bei 4 Bit:  3 (0011) → 12 (1100)
static unsigned int reverse_bits(unsigned int x, int bits) {
    unsigned int result = 0;
    for (int i = 0; i < bits; i++) {
        result = (result << 1) | (x & 1);  // niedrigstes Bit übernehmen
        x >>= 1;                           // nächste Bitstelle nachrücken
    }
    return result;
}

// Cooley-Tukey Radix-2 FFT
// → Schnelle Fouriertransformation (in-place, ohne externe komplexe Libs)
// Eingabe: buf[] enthält komplexe Zahlen (real + imag), Länge muss 2^n sein
void fft(complex_t *buf, int n) {
    // Anzahl Bits für Bit-Reversal-Berechnung (z. B. bei n=8 → 3 Bit)
    int bits = (int)log2(n);

    // Bit-Reversal-Permutation: Sortiert die Eingabedaten um
    for (int i = 0; i < n; i++) {
        int j = reverse_bits(i, bits);  // Zielindex nach Bitumkehr
        if (j > i) {                     // Nur einmal vertauschen
            complex_t tmp = buf[i];
            buf[i] = buf[j];
            buf[j] = tmp;
        }
    }

    // Hauptverarbeitung: FFT über mehrere Stufen (log2(n) Schleifen)
    for (int len = 2; len <= n; len <<= 1) {
        // Drehwinkel berechnen (für Wurzel der Einheitsrotation)
        float angle = -2 * M_PI / len;
        complex_t wlen = {cosf(angle), sinf(angle)};  // e^(-2πi / len)

        // Verarbeitung von Gruppen (je len Samples)
        for (int i = 0; i < n; i += len) {
            complex_t w = {1.0f, 0.0f};  // Startwert für komplexe Rotation
            for (int j = 0; j < len / 2; j++) {
                // Even/Odd-Zweige innerhalb des Blocks
                complex_t even = buf[i + j];
                complex_t odd  = buf[i + j + len / 2];

                // Butterfly-Berechnung: Kombination von even und odd
                float treal = w.real * odd.real - w.imag * odd.imag;
                float timag = w.real * odd.imag + w.imag * odd.real;

                buf[i + j].real             = even.real + treal;
                buf[i + j].imag             = even.imag + timag;
                buf[i + j + len / 2].real   = even.real - treal;
                buf[i + j + len / 2].imag   = even.imag - timag;

                // Rotation für nächsten Schritt vorbereiten (w = w * wlen)
                float w_real_new = w.real * wlen.real - w.imag * wlen.imag;
                w.imag = w.real * wlen.imag + w.imag * wlen.real;
                w.real = w_real_new;
            }
        }
    }
}
