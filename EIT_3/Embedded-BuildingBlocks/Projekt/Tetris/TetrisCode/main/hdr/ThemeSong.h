#ifndef THEMESONG_H
#define THEMESONG_H

#include <stdint.h>
#include "musical_score_encoder.h"

/* --------------------------------------------------------------------------
   Startet die Tetris-Theme-Melodie wiederholend auf dem Buzzer
   Implementierung muss RMT oder einen vergleichbaren Treiber verwenden
   Musik läuft standardmäßig, kann mit theme_pause()/theme_resume() kontrolliert werden
   ------------------------------------------------------------------------ */
void StartTheme(void);

/* --------------------------------------------------------------------------
   Musik pausieren (z.B. bei Game Over oder Menü)
   Verwendet Event-Group für sichere Synchronisation zwischen Tasks
   
   VERBESSERUNG: Erlaubt, Musik zu pausieren/fortsetzen ohne Task zu löschen
   Dies spart Ressourcen und verhindert Audio-Glitches beim Neustart
   -------------------------------------------------------------------------- */
void theme_pause(void);

/* --------------------------------------------------------------------------
   Musik fortsetzen (Resume nach Pause)
   -------------------------------------------------------------------------- */
void theme_resume(void);

/* --------------------------------------------------------------------------
   MULTI-SONG SYSTEM
   
   Verschiedene verfügbare Melodien können gewechselt werden
   -------------------------------------------------------------------------- */

/**
 * Theme Song Indices - Beschreibt alle verfügbaren Melodien
 * WICHTIG: Diese Reihenfolge MUSS mit dem available_songs[] Array in ThemeSong.c übereinstimmen!
 */
typedef enum {
    THEME_TETRIS = 0,
    THEME_STARWARS = 1,
    THEME_CANTINABAND = 2,
    THEME_IMPERIALMARCH = 3,
    THEME_MARIO = 4,
    THEME_HARRYPOTTER = 5,
    THEME_KEYBOARDCAT = 6,
    THEME_SILENCE = 7,
    THEME_COUNT = 8        // Gesamtanzahl der Songs
} ThemeSongIndex;

/* Wechsele zur nächsten Musik (mit Cycling: TETRIS → STARWARS → ... → SILENCE → TETRIS)
   Diese Funktion wird durch die Tastenkombination (alle 4 Buttons) aufgerufen */
void theme_next_song(void);

/* Gebe aktuelle Song-Nummer zurück */
int theme_get_current_song(void);

/* Direkter Song-Wechsel zu bestimmtem Index */
void theme_set_song(int song_index);

#endif // THEMESONG_H
