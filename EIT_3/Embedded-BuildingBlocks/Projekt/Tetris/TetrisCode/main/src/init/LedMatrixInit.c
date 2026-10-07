#include "LedMatrixInit.h"
#include "MatrixNummer.h"
#include <stdint.h>

void LedMatrixInit(uint8_t height, uint8_t width, uint16_t matrix[height][width]) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            matrix[y][x] = LED_MATRIX_NUMMER[y][x];
        }
    }
}