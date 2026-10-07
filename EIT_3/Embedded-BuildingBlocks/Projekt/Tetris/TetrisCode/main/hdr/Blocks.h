#ifndef BLOCKS_H
#define BLOCKS_H

#include <stdint.h>
#include <stdbool.h>

#define NUM_BLOCKS 7

typedef struct {
    uint8_t shape[4][4];  // 1 = Blockteil, 0 = leer
    int x;                // linke obere Ecke
    int y;                // aktuelle Position im Grid
    uint8_t color;        // Farbindex
} TetrisBlock;

// Array aller Blöcke und Rotationen
extern TetrisBlock blocks[NUM_BLOCKS][4];

// Zuweisung einer Farbe an einen Block basierend auf Typ
void assign_block_color(TetrisBlock *block, int block_type);

// RGB-Farbwerte eines Blocks abrufen
void get_block_rgb(uint8_t block_index, uint8_t *r, uint8_t *g, uint8_t *b);

// Block 90° drehen
void rotate_block_90(TetrisBlock *block);

#endif // BLOCKS_H
