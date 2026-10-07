#ifndef GRID_H
#define GRID_H

#include <stdint.h>
#include <stdbool.h>
#include "Blocks.h"
#include "Globals.h"

extern uint8_t grid[GRID_HEIGHT][GRID_WIDTH];

void grid_init(void);
bool grid_check_collision(const TetrisBlock *block);
void grid_fix_block(const TetrisBlock *block);
void grid_clear_full_rows(void);
void grid_print(void);

#endif // GRID_H
