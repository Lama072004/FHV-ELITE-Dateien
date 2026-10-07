#ifndef DISPLAY_INIT_H
#define DISPLAY_INIT_H

#include <stdint.h>
#include "lvgl.h"

extern lv_disp_t* g_disp;

// I2C und Display initialisieren
void display_init(void);

// Score und Highscore auf dem Display anzeigen
void display_update_score(uint32_t current_score, uint32_t highscore);

// Game Over Nachricht anzeigen
void display_show_game_over(uint32_t final_score, uint32_t highscore);

// Vollständigen Display-Reset durchführen und HUD mit dem übergebenen Highscore aufbauen
void display_reset_and_show_hud(uint32_t highscore);

#endif // DISPLAY_INIT_H
