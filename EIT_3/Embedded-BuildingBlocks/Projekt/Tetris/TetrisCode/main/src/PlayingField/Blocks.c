#include "Blocks.h"
#include <stdint.h>
#include <stdlib.h>
#include "Globals.h"

// Colors and NUM_BLOCKS are centralized in Globals.h / Colors.c

// Blocks mit Rotationen (4x4 Matrizen), Reihenfolge: I, J, L, O, S, T, Z
TetrisBlock blocks[NUM_BLOCKS][4] = {
    // I-Block: Hero
    {
        {.shape = {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}}, .x=0, .y=0, .color=0},
        {.shape = {{0,0,1,0},{0,0,1,0},{0,0,1,0},{0,0,1,0}}, .x=0, .y=0, .color=0},
        {.shape = {{0,0,0,0},{0,0,0,0},{1,1,1,1},{0,0,0,0}}, .x=0, .y=0, .color=0},
        {.shape = {{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0}}, .x=0, .y=0, .color=0}
    },
    // J-Block: Ricky
    {
        {.shape = {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, .x=0, .y=0, .color=1},
        {.shape = {{0,1,1,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}}, .x=0, .y=0, .color=1},
        {.shape = {{0,0,0,0},{1,1,1,0},{0,0,1,0},{0,0,0,0}}, .x=0, .y=0, .color=1},
        {.shape = {{0,1,0,0},{0,1,0,0},{1,1,0,0},{0,0,0,0}}, .x=0, .y=0, .color=1}
    },
    // L-Block: Ricky
    {
        {.shape = {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, .x=0, .y=0, .color=2},
        {.shape = {{0,1,0,0},{0,1,0,0},{0,1,1,0},{0,0,0,0}}, .x=0, .y=0, .color=2},
        {.shape = {{0,0,0,0},{1,1,1,0},{1,0,0,0},{0,0,0,0}}, .x=0, .y=0, .color=2},
        {.shape = {{1,1,0,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}}, .x=0, .y=0, .color=2}
    },
    // O-Block: Smashboy
    {
        {.shape = {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}, .x=0, .y=0, .color=3},
        {.shape = {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}, .x=0, .y=0, .color=3},
        {.shape = {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}, .x=0, .y=0, .color=3},
        {.shape = {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}, .x=0, .y=0, .color=3}
    },
    // S-Block: Rhode Island Z
    {
        {.shape = {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}, .x=0, .y=0, .color=4},
        {.shape = {{0,1,0,0},{0,1,1,0},{0,0,1,0},{0,0,0,0}}, .x=0, .y=0, .color=4},
        {.shape = {{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}}, .x=0, .y=0, .color=4},
        {.shape = {{1,0,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}}, .x=0, .y=0, .color=4}
    },
    // T-Block: Teewee
    {
        {.shape = {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, .x=0, .y=0, .color=5},
        {.shape = {{0,1,0,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}}, .x=0, .y=0, .color=5},
        {.shape = {{0,0,0,0},{1,1,1,0},{0,1,0,0},{0,0,0,0}}, .x=0, .y=0, .color=5},
        {.shape = {{0,1,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}}, .x=0, .y=0, .color=5}
    },
    // Z-Block: Cleveland Z
    {
        {.shape = {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}, .x=0, .y=0, .color=6},
        {.shape = {{0,0,1,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}}, .x=0, .y=0, .color=6},
        {.shape = {{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}}, .x=0, .y=0, .color=6},
        {.shape = {{0,1,0,0},{1,1,0,0},{1,0,0,0},{0,0,0,0}}, .x=0, .y=0, .color=6}
    }
};

void assign_block_color(TetrisBlock *block, int block_type) {
    block->color = block_type;
}

void get_block_rgb(uint8_t block_index, uint8_t *r, uint8_t *g, uint8_t *b) {
    *r = block_colors[block_index][0];
    *g = block_colors[block_index][1];
    *b = block_colors[block_index][2];
}

void rotate_block_90(TetrisBlock *block) {
    uint8_t temp[4][4];
    for(int y=0;y<4;y++)
        for(int x=0;x<4;x++)
            temp[x][3-y] = block->shape[y][x];
    for(int y=0;y<4;y++)
        for(int x=0;x<4;x++)
            block->shape[y][x] = temp[y][x];
}
