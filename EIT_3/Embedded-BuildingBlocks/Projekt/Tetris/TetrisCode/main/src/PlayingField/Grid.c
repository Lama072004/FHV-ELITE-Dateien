#include "Grid.h"
#include "Score.h"
#include "SpeedManager.h"
#include "DisplayInit.h"
#include "Globals.h"
#include "led_strip.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <string.h>
#include <stdio.h>

extern MATRIX ledMatrix;
extern led_strip_handle_t led_strip;
extern SemaphoreHandle_t led_strip_semaphore;
extern SemaphoreHandle_t score_semaphore;

uint8_t grid[GRID_HEIGHT][GRID_WIDTH] = {0};

void grid_init(void) {
    memset(grid, 0, sizeof(grid));
}

bool grid_check_collision(const TetrisBlock *block) {
    // Corner Detection: only check active (lit) pixels of the block
    for (int by = 0; by < 4; by++) {
        for (int bx = 0; bx < 4; bx++) {
            if (block->shape[by][bx] == 0) continue;  // Skip inactive pixels
            
            int gx = block->x + bx;
            int gy = block->y + by;
            
            // Check boundaries - only active pixels matter
            if (gy >= GRID_HEIGHT) return true;  // Hit bottom
            if (gx < 0 || gx >= GRID_WIDTH) return true;  // Hit left or right wall
            if (gy < 0) continue;  // Above grid is OK (spawning)
            
            // Check collision with already placed blocks
            if (grid[gy][gx] != 0) return true;
        }
    }
    return false;
}

void grid_fix_block(const TetrisBlock *block) {
    // SEMAPHOR-SCHUTZ: LED-Strip vor gleichzeitigem Zugriff schützen
    if (xSemaphoreTake(led_strip_semaphore, pdMS_TO_TICKS(50)) != pdTRUE) {
        printf("[Grid] ERROR: LED semaphore timeout in grid_fix_block\n");
        return;  // Timeout - vermeide Deadlock
    }
    
    for (int by = 0; by < 4; by++) {
        for (int bx = 0; bx < 4; bx++) {
            if (block->shape[by][bx]) {
                int gx = block->x + bx;
                int gy = block->y + by;
                if (gx >= 0 && gx < GRID_WIDTH && gy >= 0 && gy < GRID_HEIGHT) {
                    grid[gy][gx] = block->color + 1;  // Farbe speichern
                    // Write static pixel immediately so it remains lit
                    uint8_t r,g,b;
                    get_block_rgb(block->color, &r, &g, &b);
                    r = (r * GAME_BRIGHTNESS_SCALE) / 255;
                    g = (g * GAME_BRIGHTNESS_SCALE) / 255;
                    b = (b * GAME_BRIGHTNESS_SCALE) / 255;
                    int led_num = ledMatrix.LED_Number[gy][gx];
                    led_strip_set_pixel(led_strip, led_num, r, g, b);
                }
            }
        }
    }
    // Commit static pixels now
    led_strip_refresh(led_strip);
    
    xSemaphoreGive(led_strip_semaphore);  // Gib Semaphor frei
    
    // Score-Update separat schützen
    grid_clear_full_rows();
}

void grid_clear_full_rows(void) {
    // Collect all full rows first
    int remove_rows[GRID_HEIGHT];
    int remove_count = 0;
    for (int y = 0; y < GRID_HEIGHT; y++) {
        bool full = true;
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid[y][x] == 0) { full = false; break; }
        }
        if (full) {
            remove_rows[remove_count++] = y;
        }
    }

    if (remove_count == 0) return;

    // SEMAPHOR-SCHUTZ: LED-Strip für alle Operationen schützen
    if (xSemaphoreTake(led_strip_semaphore, pdMS_TO_TICKS(50)) != pdTRUE) {
        printf("[Grid] ERROR: LED semaphore timeout in grid_clear_full_rows\n");
        return;  // Timeout - vermeide Deadlock
    }

    // Blink all to-be-removed rows simultaneously
    for (int blink = 0; blink < LINE_CLEAR_BLINK_TIMES; blink++) {
        // On: set all removed-row pixels to configured color
        for (int r = 0; r < remove_count; r++) {
            int y = remove_rows[r];
            for (int x = 0; x < GRID_WIDTH; x++) {
                int led = ledMatrix.LED_Number[y][x];
                led_strip_set_pixel(led_strip, led, LINE_CLEAR_BLINK_R, LINE_CLEAR_BLINK_G, LINE_CLEAR_BLINK_B);
            }
        }
        led_strip_refresh(led_strip);
        vTaskDelay(pdMS_TO_TICKS(LINE_CLEAR_BLINK_ON_MS));

        // Off: restore static pixels for those positions based on grid
        for (int r = 0; r < remove_count; r++) {
            int y = remove_rows[r];
            for (int x = 0; x < GRID_WIDTH; x++) {
                int led = ledMatrix.LED_Number[y][x];
                // currently still grid[y][x] is non-zero, but we will clear after animation
                uint8_t rr,gg,bb;
                get_block_rgb(grid[y][x]-1, &rr, &gg, &bb);
                rr = (rr * GAME_BRIGHTNESS_SCALE) / 255;
                gg = (gg * GAME_BRIGHTNESS_SCALE) / 255;
                bb = (bb * GAME_BRIGHTNESS_SCALE) / 255;
                led_strip_set_pixel(led_strip, led, rr, gg, bb);
            }
        }
        led_strip_refresh(led_strip);
        vTaskDelay(pdMS_TO_TICKS(LINE_CLEAR_BLINK_OFF_MS));
    }

    // Now remove rows: clear those rows and apply gravity so that all blocks above fall down
    // Step 1: clear removed rows
    for (int r = 0; r < remove_count; r++) {
        int y = remove_rows[r];
        for (int x = 0; x < GRID_WIDTH; x++) grid[y][x] = 0;
    }

    // Step 2: apply gravity per column - pack non-zero cells to the bottom so no holes remain
    for (int x = 0; x < GRID_WIDTH; x++) {
        int write_y = GRID_HEIGHT - 1;
        for (int y = GRID_HEIGHT - 1; y >= 0; y--) {
            if (grid[y][x] != 0) {
                // move value down to write_y
                grid[write_y][x] = grid[y][x];
                if (write_y != y) grid[y][x] = 0;
                write_y--;
            }
        }
        // fill remaining above with zeros (if any)
        for (int y = write_y; y >= 0; y--) grid[y][x] = 0;
    }

    // Render final grid after gravity so user sees blocks settled (no holes)
    led_strip_clear(led_strip);
    for (int yy = 0; yy < GRID_HEIGHT; yy++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid[yy][x] > 0) {
                uint8_t rr,gg,bb;
                get_block_rgb(grid[yy][x]-1, &rr, &gg, &bb);
                rr = (rr * GAME_BRIGHTNESS_SCALE) / 255;
                gg = (gg * GAME_BRIGHTNESS_SCALE) / 255;
                bb = (bb * GAME_BRIGHTNESS_SCALE) / 255;
                int led = ledMatrix.LED_Number[yy][x];
                led_strip_set_pixel(led_strip, led, rr, gg, bb);
            }
        }
    }
    led_strip_refresh(led_strip);
    
    xSemaphoreGive(led_strip_semaphore);  // Gib LED-Semaphor frei
    
    vTaskDelay(pdMS_TO_TICKS(100));

    // Score and speed update: add points based on number of lines cleared simultaneously
    // SEMAPHOR-SCHUTZ: Score und Speed mit Semaphoren schützen
    if (xSemaphoreTake(score_semaphore, pdMS_TO_TICKS(100)) == pdTRUE) {
        printf("[Grid] Cleared %d lines!\n", remove_count);
        score_add_lines(remove_count);
        xSemaphoreGive(score_semaphore);
    } else {
        printf("[Grid] ERROR: Score semaphore timeout\n");
    }
    
    speed_manager_update_score(score_get_total_lines_cleared());
    display_update_score(score_get(), score_get_highscore());
}

void grid_print(void) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            printf("%d ", grid[y][x] ? 1 : 0);
        }
        printf("\n");
    }
    printf("--------------------\n");
}
