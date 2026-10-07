#ifndef SPEED_MANAGER_H
#define SPEED_MANAGER_H

#include <stdint.h>

//////////////////////////////////////////////////////////////////////////////////////////////////
// SPEED MANAGER - Dynamische Fallgeschwindigkeit basierend auf Score
//////////////////////////////////////////////////////////////////////////////////////////////////

// Initialisiert den Speed Manager (muss einmal zu Spielstart aufgerufen werden)
void speed_manager_init(void);

// Gibt die aktuelle Fallgeschwindigkeit in Millisekunden zurück
uint32_t speed_manager_get_fall_interval(void);

// Ruft dies auf, wenn der Score sich ändert (nach Zeilen)
void speed_manager_update_score(uint32_t lines_cleared);

// Setzt die Geschwindigkeit zurück auf die Start-Geschwindigkeit
void speed_manager_reset(void);

#endif // SPEED_MANAGER_H
