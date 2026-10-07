#include "SpeedManager.h"
#include "Globals.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include <stdio.h>

extern SemaphoreHandle_t speed_semaphore;

//////////////////////////////////////////////////////////////////////////////////////////////////
// SPEED PROGRESSION: Basierend auf gecleareten Zeilen
//////////////////////////////////////////////////////////////////////////////////////////////////
// Standard Tetris Speed Curve:
// Level 0:  400ms (Initial)
// Level 1:  370ms (10 Zeilen)
// Level 2:  330ms (20 Zeilen)
// Level 3:  270ms (30 Zeilen)
// Level 4:  220ms (40 Zeilen)
// Level 5:  170ms (50 Zeilen)
// Level 6:  130ms (60 Zeilen)
// Level 7:  100ms (70 Zeilen)
// Level 8:  80ms  (80 Zeilen)
// Level 9:  60ms  (90 Zeilen)
// Max:      50ms  (100+ Zeilen)

static uint32_t current_fall_interval = FALL_INTERVAL_MS;
static uint32_t total_lines_cleared = 0;

// Struktur für Speed Levels
typedef struct {
    uint32_t lines_threshold;  // Ab dieser Zeilenanzahl gilt dieser Speed
    uint32_t fall_interval_ms; // Fallgeschwindigkeit in ms
} SpeedLevel;

// Speed Progression Table (Tetris-ähnlich)
static const SpeedLevel speed_levels[] = {
    {0,   400},   
    {2,  370},   
    {5,  330},   
    {10,  270},   
    {15,  220},   
    {20,  170},   
    {25,  130},  
    {30,  100},   
    {35,  80},    
    {40,  60},    
    {45, 50},    
};

#define NUM_SPEED_LEVELS (sizeof(speed_levels) / sizeof(SpeedLevel))

// Intern: Update der Fallgeschwindigkeit basierend auf Zeilen
static void update_fall_speed(void) {
    // Finde das passende Speed Level für die aktuelle Zeilenanzahl
    for (int i = NUM_SPEED_LEVELS - 1; i >= 0; i--) {
        if (total_lines_cleared >= speed_levels[i].lines_threshold) {
            current_fall_interval = speed_levels[i].fall_interval_ms;
            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
//////////////////////////////////////////////////////////////////////////////////////////////////

void speed_manager_init(void) {
    total_lines_cleared = 0;
    // Always start with Level 0 speed from the table
    current_fall_interval = speed_levels[0].fall_interval_ms;
    printf("[SpeedManager] Initialized - Start speed: %lu ms\n", current_fall_interval);
}

uint32_t speed_manager_get_fall_interval(void) {
    // SEMAPHOR-SCHUTZ: Speed State vor gleichzeitigem Zugriff schützen
    uint32_t result = current_fall_interval;
    if (xSemaphoreTake(speed_semaphore, pdMS_TO_TICKS(10)) == pdTRUE) {
        result = current_fall_interval;
        xSemaphoreGive(speed_semaphore);
    }
    return result;
}

void speed_manager_update_score(uint32_t lines_cleared) {
    // SEMAPHOR-SCHUTZ: Speed Update mit Semaphor schützen
    if (xSemaphoreTake(speed_semaphore, pdMS_TO_TICKS(10)) != pdTRUE) {
        printf("[SpeedManager] ERROR: Speed semaphore timeout\n");
        return;
    }
    
    uint32_t old_speed = current_fall_interval;
    total_lines_cleared = lines_cleared;  // Grid.c tracked the total, just use it
    update_fall_speed();
    
    printf("[SpeedManager] Lines cleared: %lu total, Current speed: %lu ms\n", 
           total_lines_cleared, current_fall_interval);
    
    if (current_fall_interval != old_speed) {
        printf("[SpeedManager] ⚡ LEVEL UP! Lines: %lu, Speed: %lu ms (was %lu ms)\n", 
               total_lines_cleared, current_fall_interval, old_speed);
    }
    
    xSemaphoreGive(speed_semaphore);
}

void speed_manager_reset(void) {
    total_lines_cleared = 0;
    // Reset to Level 0 speed from the table
    current_fall_interval = speed_levels[0].fall_interval_ms;
    printf("[SpeedManager] Reset - Speed: %lu ms\n", current_fall_interval);
}
